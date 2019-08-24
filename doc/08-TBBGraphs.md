# Threaded Building Block Graphs

## Graphs

The most general kind of task based workflow is a graph, where tasks
are nodes and edges are data that flows between the nodes.

```
+---------+
|  Start  | <- data1, data2, data3, ..., dataN
+---------+---------------
    |      \              \
    \/       \             \
+---------+   \|            \|
| Node 1  |    +--------+    +--------+
+---------+    | Node 2 |    | Node 4 |
    |          +--------+    +--------+
    \/             |             |
+---------+        |             |
| Node 3  |        |             |
+---------+        |             |
    |       +------+-------------+
    \/     |/
+---------+
|  Node 5 | -> out1, out2, out3, ..., outN
+---------+
```

This is a very flexible way to execute tasks in parallel and TBB has many node types the allow for processing, splitting, joining, queueing etc. Note also that although the ASCII art example there is a DAG (*Directed Acyclic Graph*), TBB can implement graphs with cycles as well.

See the [Intel documentation](https://software.intel.com/en-us/node/517340) as a good introduction.

### TBB Graph Basics

#### Setting Up a Graph

A graph in TBB is an object of type `tbb::flow::graph`, and you need
to include the header `tbb/flow_graph.h`:

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

3. A callable object, which is actually going to be run by TBB when the node is given something to do. The callable has to take the input data type as an argument and must return the output data type. As with TBB's other parallel constructs, the callable can be a lambda function in the simplest cases (e.g., a small pure function) or can be a class that implements `operator()`.

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

Note that all messages passed between graph nodes are *copied*. So, they must be copiable objects and it's best that they are not large objects. If large pieces of data need to be passed between nodes then we can use a pointer - `unique_ptr` will not work (it's not copyable), but `shared_ptr` does work.

If you do use a shared pointer as an input to a `function_node` then it should be a `const`, e.g.,

```cpp
tbb::flow::function_node< shared_ptr<T>, shared_ptr<T> >
    generator( g, tbb::flow::unlimited, [](const shared_ptr<T> v) {
        shared_ptr<T> v2 = make_shared<T> (some_function(*v));
        return v2;
} );
```

If you don't actually require to pass data in your message, but just tell a node to *go*, then you can use the lightweight message `tbb::flow::continue_msg()`.

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

A source node takes no input, but generates output internally, passing it out to its connected nodes. A source node needs to provide a callable that accepts a reference to its data type and sets the value of the reference to the data to be passed to other nodes. The call interface itself returns a boolean: `true` if more data might be available, `false` if not.

This source node provides the lines from a file as messages, one by one:

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

tbb::flow::source_node<std::string> input_node(g, file_cat(my_stream_p), false);

...

input_node.activate();
```

Note that the third parameter to the source node constructor determines if the source node is immediately *active*, in which case it will start sending messages immediately. However, if the graph is not yet fully constructed this might not result in the correct behaviour, so it can also be set to `false`. In that case, the source node is activated by calling its `activate` method.

#### Buffers, Limiters, Queues, etc.

Depending on the graph you want to build, TBB offers queuing nodes (with a priority queue), buffering nodes, splitting nodes and joining nodes. However, these are probably less useful in HEP where our workflow tends to be representable as a DAG, with simple edges between nodes.

### Combining Parallelism

One of the nice features of TBB is that the internal thread pool is managed between all types of parallelism in an efficient way. This means that you can (and *should*) use parallelism within a TBB graph node, if that's possible.

In this example a node that processes an array of doubles into another array of doubles uses `parallel_for` to exploit the concurrency available in this operation.

```cpp
function_node< double *, double * > n1( g, unlimited, [&]( double *a ) -> double * {
    double *b = new double[N];
    parallel_for( 0, N, [&](int i) {
        b[i] = f1(a[i]);
    } );
    return b;
} );
```

(As you can see, lambdas can be nested!)

# Exercises

1. Write a simple TBB graph example that prints its input in one node, then passes the input to a second node that squares the number and prints that. Use `try_put()` to inject numbers into the graph.
    1. Now add a third node, which takes the output of the first node and cubes it. Run this node in parallel with the `n^2` node.
    2. Add a fourth node that takes the inputs from the `n^2` and `n^3` nodes and sums them up.
    3. Instead of using `try_put` to inject data, write a simple `source_node` that injects some numbers.
    4. You might extend the example by adding multiple paths through the DAG (multiple output edges from a node).



3. In file `strip_det.hpp` you'll find a class for a small *fooble* strip detector (`det_strip`) consisting of a vector of cells (`det_cell`). Various methods are defined to do noise suppression, calculate data quality, extract the signal and detect foobles. There is also a method to serialise the strip data from a file. In this exercise we'll use a TBB graph to do data preparation and see if we have found a fooble.
    1. Write a `source_node` that will load the detector data from a file.
        1. You can use the `det-rand-dump.cc` code to generate data (it will write to `fooble.txt`).
        1. Just to test that the data is loaded correctly, attach a node to the source that prints some basic information on the strip, e.g., the number of cells.
    2. Now, for each strip calculate the data quality value (`det_strip.data_quality()`) in a graph node (note the class caches this value internally).
    3. Use a node to calculate the signal (`det_strip.signal()`).
    4. In parallel, look for foobles (`det_strip.fooble_signal()`).
        1. To see if you have found a fooble - make a DQ cut (`dq > 0.9`) and see if the strip has `fooble_signal()` true.
        2. You need a way to count the total number of foobles detected, for a later step.
    5. After the graph has run, if you find more than 20% of the detector saw a fooble then you have a real fooble.
    6. Add a data quality histogrammer, that makes a simple histogram of the data quality and signal values for the detector (*hint* all the strips are positioned in `[0, 1)`).
    7. Write that histogram to a stdout.
        1. Is it possible to do that as part of the graph?
    8. Is it possible to parallelise any of the graph steps themselves?
        1. If you find some, then try to do this.
