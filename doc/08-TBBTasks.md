# Threaded Building Block Tasks

## The Task Manager

So far we've seen how TBB allows us to express concurrency by
applying simple parallel programming patterns, like the 
`parallel_for` and `parallel_reduce`. The use of TBB's pipelines
and graphs allwed more sophisticated workflows and
data processing operations to be constucted that mixed
in task based parallelism.

Underlying the execution of all of these workflows is the 
TBB *task scheduler*, where these higher level patterns are
broken down into work units that are executed in the thread
pool that TBB created when the program started.

Sometimes, however, these higher level patterns do not manage
to adequately capture the nature of our problem. In that case
there is the option to submit work tasks directly to the TBB
task scheduler. This is what we will look at here.

### *Caveat Emptor*

Before diving into using the task scheduler directly, do think
hard about your problem and search for ways to express it in
more familiar parallel patterns. When that is done, TBB takes care
of efficiently breaking down the problem into appropriate tasks.
Only if you find that's really not
possible should you consider direct task spawning and here you
inherit the responsibility yourself of spawing appropriate
sized tasks to exploit the hardware that you are running on.

## Task Groups

A simplified inteface to the task manager is available through
the *task group* class. The task group class can marshall a
number of tasks and wait for them to complete.

Here's a simple example:

```cpp
long Fib(long n) {
  if( n<2 ) {
    return n;
  } else {
    long x, y;
    tbb::task_group g;
    g.run([&]{x=Fib(n-1);}); // spawn a task
    g.run([&]{y=Fib(n-2);}); // spawn another task
    g.wait();                // wait for both tasks to complete
    return x+y;
  }
}
```

This simple recursive Fibonacci solver spawns two tasks, to
calculate the two prevoious Fibonacci numbers. Each of these
tasks creates another task group, finding its two previous
numbers (with the obvious cut off for the lower bound).

In this example task creation is also recursive, which prevents
the creation of tasks itself from becoming a serial bottleneck.

### Cancelling work

If you have a large task group that you can decide no longer needs
to be run, use the `cancel()` method.

Cancelling tasks that are actually running is not possible to do
safely (state corruption could easily arise), so `cancel()` only
prevents queued tasks from starting.

## Low Level Task Manager Interface

Task groups are relatively simple interface to the task manager,
TBB also offers an API that allows for a much lower level
interaction with the task manager, with fine control over the 
per-thread and global task queues and the dependency relationships
between tasks. However, using this interface is really for
advanced or special use cases and beyond what we can cover
in this tutorial. You may want to peruse the TBB
[documentation](https://software.intel.com/en-us/node/506294)
on this topic if you think it would be useful for you.


# Exercises

1. Write a recursive maze solver using task groups. The number
   of turns in the maze is known (say 10), and at each junction
   a left or a right turn can be taken.
2. Using a trivial operation that consumes time (even `sleep`
   will do), write a small
   application that spawns more tasks than can start at once,
   then use `cancel()` before all the tasks have started.
     1. Try to show that running tasks continue until they exit

