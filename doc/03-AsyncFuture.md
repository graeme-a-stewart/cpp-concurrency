# Async Tasks and Futures

## Introduction

Running threads directly from `std::thread` does have some drawbacks, particularly because there's no direct way to get the return value of the function the thread executed (which in scientific computing it often what we really want). Using messages or shared objects may not be the safest or easiest way of doing this. However, the standard library offers some more convenient ways to manage this simple case.

## Async Tasks and Futures 

An `async` task is very like launching a thread - it has the same syntax, giving the name of the function to call and any arguments it should take. However, the return value from an `async` task is a special class template, a `future`, in which the return value will
only be placed once the task has executed and returned.

So, we just launch a thread as an `std::async` object and assign the return value to the `std::future` templated class. E.g.,

```cpp
#include <future>

double some_expensive_operation(std::vector<thing>& stuff) {
    // Do expensive things to stuff
    ...

    return result;
}

int main() { auto my_stuff = initialise_stuff();

    std::future<double> res = std::async(some_expensive_operation, std::ref(my_stuff));

    // do some other things

    double the_answer = res.get();
    ...
```

You can see that the value of the variable is accessed with the `get()` method of the `future`.

A future also has the `wait()` method, which will block the calling thread until the result of the asynchronous execution is available, but it doesn't read the result.

### Launch Parameter

If you run an `async` task, you might be surprised that it doesn't run in the background -- it might well run only when the future's get method is called. This is because the implementation can define whenit runs an asynchronous task, either immediately or deferred. However, if the first parameter passed to the invocation of the task is an `std::launch` parameter, this can be controlled manually.

In particular `std::launch::async` says that the asynchronous thread should execute immediately, where as `std::launch::deferred` will delay execution until either `wait()` or `get()` is called on the future. The default value is `std::launch::async | std::launch::deferred`, which is why the implementation can choose what happens.

Obviously using `std::launch::async` is what should be done if you know there's no point in deferring execution. An advantage of `std::launch::deferred` is that if it turns out that the async task isn't needed then it just won't be run at all when the future goes out of scope.

## Packaged Tasks and Promises

There are other ways to set the values associated with a `future`. One is to setup a `packaged_task` that allows a task to be tied to a `future` before it is sent off for dispatch - this is useful if you have a task queue, but need to make sure you can get a handle on the result of the function before it is sent to the queue.

The other is to extract a `promise` from an already created `future` and send it as a parameter elsewhere in the program. When the `promise` is set the value of the `future` it came from becomes available.

# Exercises

1. Write a small program that executes some numerical calculation using an `std::async` task to return the value as an `std::future`. (You might use the classic monte-carlo determination of pi, picking a random `x` and `y` in [0, 1] and seeing if the point is within a circle of radius 1. (The ratio of points in the circle to the total number of points is pi/4.)
    1. Does the task run asynchronously without an `std::launch` parameter?
    2. Use the `std::launch` parameter to control execution and force the task to run asynchronously.
    3. Can you demonstrate that a task launched with `deferred` where the future is not interrogated actually never runs?

2. Generalise the above exercise and use `std::async` to fill all the cores on your machine.
    1. You can use `std::thread::hardware_concurrency` to allow the program to estimate the correct number of threads to launch.
