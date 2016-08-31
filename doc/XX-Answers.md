# Solutions

Listed here are programs that are solutions to the exercises posed in the other tutorial documents. You can compare your solution to them and also use them if you get stuck.

You'll find the programs in https://github.com/graeme-a-stewart/cpp-concurrency, under `src/cpp11` for 01-03 and `src/tbb` for 04-07.

## 01 Basic Thread Management

1. `start_threads.cc`
    1. `start_threads.cc` (manual) and `start_threads_vec.cc` (using
       `std::vector`)

2. `thread-arguments.cc`

3. `thread-arguments-problems-fixed.cc` and
   `thread-arguments-problems2-fixed.cc`

4. `hw-concurrency.cc`

## 02 Mutexes and Atomics

1. `thread-arguments-mutex.cc`

2. `multithread-sum-fixed-atomic.cc` and 
   `multithread-sum-fixed-mutex.cc`. 

3. See `multithread-sum-fixed-atomic-efficient.cc` and
   `multithread-sum-fixed-mutex-efficient.cc` and compare with the
   versions above.
   
4. `deadlock-fixed.cc` and
   `deadlock-fixed-using-lock-adoption.cc`

## 03 Async and Futures

1. `async_tasks.cc`

2. `async_tasks_multi.cc`


## 04 TBB Loops

1. `parallel_for_basic.cc`

2. `parallel_for_lambda.cc`

3. `parallel_reduce.cc`

4. `parallel_mandel.cc` (see `parallel_mandel_vector.cc` for the same donw with `std::vector` storage)

## 05 TBB Containers

1. `concurrent_vector.cc` and `concurrent_vector_stdthread.cc` (see also `concurrent_vector_grow_by.cc`)

2. `generate_and_filter.cc`

## 06 TBB Pipelines

1. `maths_pipeline.cc`

## 07 TBB Graphs

N.B. These examples are in the `graph` subdirectory

1. `data_flow.cc` and `data_flow_ptr.cc` (which uses `shared_ptr`).

    * See `dep_graph.cc` and `dep_graph_broadcast.cc` for multiple data paths a la tee

2. `det_data_proc.cc`.
