Pipelines and Graphs
=

Introduction
-

Many of the problems we face in writing concurrent code are addressed
directly by the TBB loop functions `parallel_for`, `parallel_reduce`
and other
[more sophisticated patterns](https://software.intel.com/en-us/node/506140).

However, TBB also allows for parallelisation based on *graphs* of
tasks, where tasks are nodes and data flows along edges between them.

This is a very flexible way to execute in parallel...


Pipelines
-

*Pipelines* are a simpler execution concept. Here the tasks to be
 executed come in a linear sequence, much like an assembly line. Each
 incoming piece of data is processed by the first element of the
 pipeline, then the second and so on. Obviously this is a serial
 process for any data element. Parallelism arises because we can have
 many pieces of data moving through the pipeline at the one time.

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
	| Stage 1 |
	+---------+
	    |
		\/
	+---------+
	|  Stop   | -> out1, out2, out3, ..., outN
	+---------+
```

In TBB each stage can either be serial or parallel. For serial stages
no more than one element will be processed by this stage at a
time. Serial stages can be in order or out of order. For parallel
stages, multiple elements can be processed at the same time and the
ordering is obviously not guaranteed.

Considering the performance one might hope to get with a pipeline,
obviously the serial stages are bottlenecks (especially *serial in
order*), so keeping these stages short will help a lot.

### Defining a Pipeline

Pipelines are defined as a series of *filters*, each of which takes
some input data and produces some transformed output data. The first
filter's input data is `void`, as is the last filter's output data.

```cpp
    void RunPipeline( int ntoken, FILE* input_file, FILE* output_file ) {
        tbb::parallel_pipeline(
            ntoken,
            tbb::make_filter<void,Data*>(
                tbb::filter::serial_in_order, InputFunc(input_file) )
        &
            tbb::make_filter<Data*,Data*>(
                tbb::filter::parallel, TransformationFunc() )
        &
            tbb::make_filter<Data*,void>(
                tbb::filter::serial_in_order, OutputFunc(output_file) );
    }
```

In the above case the first filter is `InputFunc`, which is
instantiated with an input file pointer. The next filter is
`TransformationFunc` and then finally the last filter is `OutputFunc`,
which is also instantiated with a file pointer.

Note that we read the input file serially and in order, doing the same
for the output file. However, the transformation function can run in
parallel.

Notice that the `&` operator is used to concatenate filters and the
`make_filter` template is used to specify the data ingested and
produced by each step. Naturally the output data of one step has to be
same as the input data for the next.

The very first parameter we passed to `tbb::parallel_pipeline` is
`ntoken`, which specifies the maximum number of pieces of data that
can be processed. It's necessary to have such a limit or data will
pile up at the choke point of the filter and consume resources
needlessly.

### How a Pipeline Runs

TBB actually runs the pipeline by calling the `()` operator of each
filter for the data element that's going to be processed. This
operator has to take the correct *input* data type, as specified in
the `make_filter` template, and produce the correct *output* datatype.

