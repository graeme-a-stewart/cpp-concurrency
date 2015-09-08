# Pipelines and Graphs

## Introduction

Many of the problems we face in writing concurrent code are addressed directly by the TBB loop functions `parallel_for`, `parallel_reduce` and other [more sophisticated patterns](https://software.intel.com/en-us/node/506140).

However, TBB also allows for parallelisation based on task workflows, which we will look at now.

## Pipelines

*Pipelines* are a simple execution concept. Tasks to be executed come in a linear sequence, much like an assembly line. Each incoming piece of data is processed by the first element of the pipeline, then the second and so on. Obviously this is a serial process for any data element. Parallelism arises because we can have many pieces of data moving through the pipeline at the one time.

```
+---------+
|  Start  | <- data1, data2, data3, ..., dataN
+---------+
    |
    \/
+---------+
| Stage 1 |
+---------+
    |
    \/
+---------+
| Stage 2 |
+---------+
    |
    \/
+---------+
|  Stop   | -> outN, ... , out3, out2, out1
+---------+
```

In TBB each stage can either be serial or parallel. For serial stages no more than one element will be processed by this stage at a time. Serial stages can be in order or out of order. For parallel stages, multiple elements can be processed at the same time and the ordering is obviously not guaranteed.

Considering the performance one might hope to get with a pipeline, obviously the serial stages are bottlenecks (especially *serial in order*), so keeping these stages short will help a lot.

### Defining a Pipeline

Pipelines are defined as a series of *filters*, each of which takes some input data and produces some transformed output data. The first filter's input data is `void`, as is the last filter's output data.

```cpp
void RunPipeline(int ntoken, FILE* input_file, FILE* output_file) {
    tbb::parallel_pipeline(
        ntoken,
        tbb::make_filter<void,double>(
            tbb::filter::serial_in_order, DataReader(input_file) )
    &
        tbb::make_filter<double,double>(
            tbb::filter::parallel, Transform() )
    &
        tbb::make_filter<double,void>(
            tbb::filter::serial_in_order, DaraWriter(output_file) );
}
```

In the above case the first filter is `DataReader`, which is instantiated with an input file pointer. The next filter is `Transform` and then finally the last filter is `DaraWriter`, which is also instantiated with a file pointer.

Note that we read the input file serially and in order, doing the same for the output file. However, the transformation function can run in parallel.

Notice that the `&` operator is used to concatenate filters and the `make_filter` template is used to specify the data ingested and produced by each step. Naturally the output data of one step has to be same as the input data for the next.

The very first parameter we passed to `tbb::parallel_pipeline` is `ntoken`, which specifies the maximum number of pieces of data that can be processed. It's necessary to have such a limit or data will pile up at the choke point of the pipeline and consume resources needlessly.

### How a Pipeline Runs

TBB actually runs the pipeline by calling the `()` operator of each filter for the data element that is going to be processed. This operator has to take the correct *input* data type, as specified in the `make_filter` template, and produce the correct *output* datatype.

e.g., our `Transform` class might look like this:

```cpp
class Transform {
public:
    double operator()(double const number) const {
        double answer=0.0;
        if (number > 0.0)
            answer = some_expensive_calculation(number)
        return answer;
    }
};
```

Note that as the `()` operator might be called on a copy of the original class instance it needs to be `const` to ensure it does not make changes to the body which would be lost in the copy.

The DataWriter class can be similarly simple:

```cpp
    class DataWriter {
    private:
        FILE* my_output;
    
    public:
        DataWriter(FILE* out):
            my_output{out} {};
    
        void operator()(double const answer) const {
            fprintf(my_output, "%lf\n", answer);
        }
    };
```

The first class has to be slightly more complicated, because it has the responsibility for telling the pipeline when there is no more data to be processed. In this case the pipeline will call the first filter with a special `tbb::flow_control` object reference. When there is no more data to be processed then the filter should call the `stop()` method of the `flow_control` object before returning.

```cpp
class DataReader {
private:
    FILE *my_input;

public:
    DataReader(FILE* in):
        my_input{in} {};

    DataReader(const DataReader& a):
        my_input{a.my_input} {};

    ~DataReader() {};

    double operator()(tbb::flow_control& fc) const {
        double number;
        int rc = fscanf(my_input, "%lf\n", &number);
        if (rc != 1) {
            fc.stop();
            return 0.0;
        }
        return number;
    }
};
```

Note also that the first filter *must* provide a copy constructor, because it will be copied when defining and running the pipeline.

## Graphs

The most general of task workflow is a graph, where tasks are nodes and edges are data that flows between the nodes.

```
 +---------+
|  Start  | <- data1, data2, data3, ..., dataN
+---------+
    |      \
	\/       \
+---------+   \|
| Node 1  |    +--------+
+---------+    | Node 2 |
    |          +--------+
	\/             |
+---------+        |
| Node 3  |        |
+---------+        |
    |       +------+ 
	\/     |/
+---------+
|  Node 5 | -> out1, out2, out3, ..., outN
+---------+
```

This is a very flexible way to execute tasks in parallel, and TBB offers very good support for them (see https://software.intel.com/en-us/node/517340 as an introduction).

### TBB Graph Basics

#### Setting Up a Graph

A graph in TBB is an object of type `tbb::flow::graph`, and you need to include the header `tbb/flow_graph.h`:

```cpp
    #include "tbb/flow_graph.h"

    int main() {
        tbb::flow::graph g;
        ...
```

#### Attaching Nodes to your Graph

In TBB nodes can be of different types, but let's use a `function_node` for now. This is a basic data processing node that can ingest a certain type of data and then will produce an output data type. (Note, in TBB-speak, nodes exchange *messages*, but these messages can be meaningful pieces of data.)

To define a function node we use the function node template function:

```cpp
tbb::flow::function_node< float, float > squarer( g, tbb::flow::unlimited, [](const float &v) { 
    return v*v; 
    } );
```

The template function needs to know the input and output data types for this node - these are the template parameters. Then the constructor takes as parameters:

1. A reference to the graph where this node is going to be created (here, `g`).

2. A *parallelism* parameter, that tells TBB how many of these node types can be run in parallel (in this case we use the special parameter `tbb::flow::unlimited` that tells TBB this function can be run as many times in parallel as desired).

3. A callable object, which is actually going to be run by TBB when the node is given something to do. The callable has to take the input data type as an argument and must return the output data type.

As with TBB's other parallel constructs, the callable can be a lambda function in the simplest cases (e.g., a small pure function) or can be a class that implements `operator()`.

#### Making Edges

Obviously we need to connect nodes with edges so that the graph can do some useful work. This is easily done with the `tbb::flow::make_edge` function. It takes an input and an output node and connects them together:

```cpp
tbb::flow::make_edge(n1, n2);
```

This will connect node `n1` to `n2`.

#### Getting The Graph Started

You can push data into the graph by using the `try_put` method of a node. This pushes a piece of data into the node and will then trigger all of the associated data flows.

```cpp
n1.try_put(1.0);
```

Note that `try_put` returns a boolean to say if the node accepted the data.

#### At the End

Before the graph object goes out of scope, we need to wait for all data processing to finish by calling the `wait_for_all()` method.

```cpp
g.wait_for_all()
```

#### Minimal Example

This is a trivial example, but illustrates how two nodes process data through a simple graph.

```cpp
#include <iostream>

#include "tbb/tbb.h"
#include "tbb/flow_graph.h"

int main() {
    tbb::flow::graph g;

    tbb::flow::function_node<int, int> n(g, tbb::flow::unlimited, []( int v ) -> int { 
        tbb::this_tbb_thread::sleep(tbb::tick_count::interval_t(0.1*v));
        std::cout << "node n " << v << std::endl;
        return v;
    } );

    tbb::flow::function_node<int, int> m(g, 1, []( int v ) -> int {
        v *= v;
        tbb::this_tbb_thread::sleep(tbb::tick_count::interval_t(0.5));
        std::cout << "node m " << v << std::endl;;
        return v;
    } );


tbb::flow::make_edge(n, m);
for (int i=0; i<10; ++i) {
    if (!n.try_put(i)) {
        std::cerr << "Failed to put " << i << std::endl;
    }
}

g.wait_for_all();

return 0;
}
```

Note that although we check the return code of `try_put` here, if a node's parallelism has been exhausted then by default TBB will buffer input messages until the node is ready to receive another input. (Nodes can be set to reject messages that they cannot process immediately, which allows you to have graphs that manage a *now or never* workflow.)

Although the final node, `m`, does produce some data, as nothing reads this it's just discarded.

#### Copies and Messages

Note that all messages passed between graph nodes are *copied*. So, they must be copiable objects and it's best that they are not large objects. If large pieces of data need to be passed between nodes then we can use a pointer (`unique_ptr` will not work (it's not copiable), but `shared_ptr` should work).

### Further Node Types

#### Broadcast Node

A broadcast node sends its input to any output node to which it's connected -- it doesn't do any processing. (It's a bit like a plumbing T pipe.)

```cpp
tbb::flow::graph g;

tbb::flow::broadcast_node<std::string> shout(g);
tbb::flow::function_node<std::string, std::string> en2fr(g, 1, translate_1);
tbb::flow::function_node<std::string, std::string> en2de(g, 1, translate_2);

tbb::flow::make_edge(shout, en2fr);
tbb::flow::make_edge(shout, en2de);
```

#### Source Node

A source node takes no input, but generates output internally, passing
it out to its connected nodes. A source node needs to provide a
callable that accepts a reference to it's data type and sets the value
of the reference to the data to be passed to other nodes. The call
interface itself returns a boolean: `true` if more messages are
available, `false` if not.

This source node provides the lines from a file as messages, one by
one:

```cpp
class file_cat {
private:
    std::ifstream* m_input_stream_p;
public:
   file_cat(std::ifstream* input_stream_p):
     m_input_stream_p(input_stream_p) {};

     bool operator() (std::string& msg) {
        *m_input_stream_p >> msg;
        if (m_input_stream_p->good())
            return true;
        return false;
     }
 };

...

tbb::flow::source_node<std::string> input_node(g, my_stream, false);

...

input_node.activate();
```

Note that the third parameter to the source node constructor determines if the source node is immediately *active*, in which case it will start sending messages immediately. However, if the graph is not yet fully constructed this might not result in the correct behaviour, so it can also be set to `false`. In that case, the source node is activated by calling its `activate` method.

#### Buffers, Limiters, Queues, etc.

Depending on the graph you want to build, TBB offers queuing nodes (with a priority queue), buffering nodes, splitting nodes and joining nodes. However, these are probably less useful in HEP where our workflow tends to be representable as a DAG, with simple edges between nodes.

### Combining Parallelism

One of the nice features of TBB is that the internal thread pool is managed between all types of parallelism in an efficient way. This means that you can (and *should*) use parallelism within a TBB graph node, if that's possible.

**EXAMPLE HERE**

# Exercises

1. Using the model functions about, write a TBB pipeline that will:
    1. Read numbers from a file in the input filter, converting them to doubles.
    2. Run an (expensive) operation on them in the second filter, which you should run in parallel.
    3. Output the transformed numbers to an output file.
    1. See how the speed up you achieve varies with the number of tokens in the pipline. You can use the `gen_input.cc` program to generate a suitable set of input values for the pipeline.

2. Extend your pipeline with a few more steps.
    1. You can try changing the data type along the pipeline, e.g., moving from double to float, or even moving from double to some annotated `struct`.
    2. Can you introduce a monitoring step in the pipeline, which computes some aggregate property of the data (e.g., average). Can you make this step parallel and avoid data races?

3. Write a simple TBB graph example that prints its input in one node, then passes the input to a second node that squares the number and prints that. Use `try_put()` to inject numbers into the graph.
    1. Now add a third node, which takes the output of the first node and cubes it. Run this node in parallel with the `n^2` node.
    2. Add a fourth node that takes the inputs from the `n^2` and `n^3` nodes and sums them up.
    3. Instead of using `try_put` to inject data, write a simple `source_node` that injects some numbers.

4. In file `write_me.cc` you'll find a small data object class, which
   stores the input data for a small strip detector. This exercise is
   to build up a TBB graph that will process the data in the following way:
    1. Read input data for OBJECTS from FILE.
    2. Calculate a data quality measure for each strip. (N.B. this is a method already in the class, but can you parallelise it with TBB?)
   	3. Apply the noise reduction method of the class to normalise the data.
    4. Apply data processing steps `A` and `B`.
    5. Pass the data through a monitor that calculates the detector occupancy.
    6. Re-serialise the output data to a file.
