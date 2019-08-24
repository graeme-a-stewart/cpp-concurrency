# Solutions

Listed here are programs that are solutions to the exercises posed in the other tutorial documents. You can compare your solution to them and also use them if you get stuck.

You'll find the programs in
https://github.com/graeme-a-stewart/cpp-concurrency, under `src/cpp11` for
01-03 and `src/tbb` for 04-09. There is now a subdirectory for each section of
the tutorial.

## 01 Basic Thread Management

1. `start-threads.cc`
    1. `start-threads.cc` (manual) and `start-threads-vec.cc` (using
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

1. `async-tasks.cc`

2. `async-tasks-multi.cc`


## 04 TBB Loops

1. `parallel-for-basic.cc`

2. `parallel-for-lambda.cc`

3. `parallel-reduce.cc`

4. `parallel-minimisation.cc`

5. `parallel-mandel.cc` (see `parallel-mandel-vector.cc` for the same done with `std::vector` storage)

## 05 Mutexes and Atomics (TBB Version)

1. `thread-arguments-mutex.cc`

2. `multithread-sum-fixed-atomic.cc` and 
   `multithread-sum-fixed-mutex.cc`. 

3. See `multithread-sum-fixed-atomic-efficient.cc` and
   `multithread-sum-fixed-mutex-efficient.cc` and compare with the
   versions above.
   
4. `deadlock-fixed.cc` and
   `deadlock-fixed-using-lock-adoption.cc`

## 06 TBB Containers

1. `concurrent-vector.cc` and `concurrent-vector-stdthread.cc` (see also `concurrent-vector-grow-by.cc`); `concurrent-vector-stl.cc` is broken

2. `generate-and-filter.cc`

## 07 TBB Pipelines

1. `maths-pipeline.cc`

## 08 TBB Graphs

N.B. These examples are in the `graph` subdirectory

1. `data-flow.cc` and `data-flow-ptr.cc` (which uses `shared_ptr`).

    * See `dep-graph.cc` and `dep-graph-broadcast.cc` for multiple data paths a la tee

2. `det-data-proc.cc`.

## 09 TBB Tasks

N.B. These examples are in the `tasks` subdirectory

1. `maze.cc`

2. `cancel-sleep.cc`

