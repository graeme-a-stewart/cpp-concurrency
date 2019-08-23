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
            tbb::filter::serial_in_order, DataWriter(output_file) );
}
```

In the above case the first filter is `DataReader`, which is instantiated with an input file pointer. The next filter is `Transform` and then finally the last filter is `DataWriter`, which is also instantiated with a file pointer.

Note that we read the input file serially and in order, doing the same for the output file. However, the transformation function can run in parallel.

Notice that the `&` operator is used to concatenate filters and the `make_filter` template is used to specify the data ingested and produced by each step. Naturally the output data of one step has to be same as the input data for the next.

The very first parameter we passed to `tbb::parallel_pipeline` is
`ntoken`, which specifies the maximum number of pieces of data that
can be processed simultaneously. It's necessary to have such a limit
or data will pile up at the choke point of the pipeline and consume
resources needlessly.

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

# Exercises

1. Using the model functions about, write a TBB pipeline that will:
    1. Read numbers from a file in the input filter, converting them to doubles.
    2. Run an (expensive) operation on them in the second filter, which you should run in parallel.
    3. Output the transformed numbers to an output file.
    1. See how the speed up you achieve varies with the number of tokens in the pipline. You can use the `gen_input.cc` program to generate a suitable set of input values for the pipeline.

2. Extend your pipeline with a few more steps.
    1. You can try changing the data type along the pipeline, e.g., moving from double to float, or even moving from double to some annotated `struct`.
    2. Can you introduce a monitoring step in the pipeline, which computes some aggregate property of the data (e.g., average). Can you make this step parallel and avoid data races?

