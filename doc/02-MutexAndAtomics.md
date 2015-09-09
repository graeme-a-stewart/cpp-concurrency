Title: Data Access, Mutexes and Atomics
Author: Graeme A Stewart

# Data Access, Mutexes and Atomics

## Introduction

Multiple threads share the heap memory space of their process (they have separate stack memory which is what gives them their thread execution context). This can be an advantage as all threads can see the memory of common heap objects and operate on it without requiring inter-process communication or other overheads. However, this can also lead to trouble, as now threads can easily interfere with each other by accessing objects simultaneously or changing memory values they shouldn't.

In general, simultaneous read access to resources is not a problem - as data is only read then there's no conflict in multiple threads retrieving a value at once. However, write access is an issue. If two threads write to a value in an uncontrolled way we have a *data race*, where the final value will depend on the timing of the execution of the two threads (this is usually undesirable behaviour, though there are exceptions). Worse, if the object being written to is more complex than a simple type, then the state of the object may be left invalid by the race condition between the two threads. e.g., a linked list consists of some data object and pointers to the previous data item and the next data item; simultaneous insertion of two elements can easily lead to corruption of the list. This problem with data being left in a partially updated state means that write access to data can also lead to problems when reading the object while it's in an invalid intermediate state. So, we need to be careful of data races between read and write threads as well.

## Mutexes and Data Races

Control of access to critical parts of the code, where races can occur, is usually achieved with a *mutex*, which gives mutually exclusive access to some section of the code. The different threads share the mutex object, but only one of them can lock it at any one time.

```cpp
#include <mutex>

std::mutex mtx;

void some_thread () {
    mtx.lock();
    // some work while we have exclusive access
    ...
    mtx.unlock()
}
```

Notice that mutexes are defined in the `mutex` header and are part of the `std` namespace.

Naturally, as the programmer, you have to decide which mutexes control access to what parts of the program.

The above pattern can work well, but it might have problems if the work code can raise exceptions, because you have to be very careful to make sure you unlock the mutex when you are finished with it; e.g., the following code definitely has a problem when an exception is raised:

```cpp
try {
    mtx.lock();

    // some work that might raise an exception
    ...
    mtx.unlock();
}
catch (std::exception) {
    // Handle exception
    ...
}
```

Now, obviously one can restructure the code in this simple case, but what happens if multiple parts of the code can raise an exception? or if the exception handling is outside this function? Things quickly can become complicated and error prone.

A much better technique is to use the RAII paradigm (*Resource Acquisition Is Initialisation*), where we create an object that embodies the locked state, releasing the lock in the destructor. That would look something like this:

```cpp
class raii_lock {
private:
    std::mutex *m_mtx;

public:
    lock_guard(std::mutex& mtx):
    {
        m_mtx = &mtx;
        m_mtx->lock();
    }

    ~lock_guard() {
        m_mtx->unlock();
    }
};
```

In fact this pattern is so useful that it is supported directly in C++11 as a `lock_guard` (also part of the `mutex` header). As there are a few different types of mutexes one can use, the `lock_guard` is a templated class. Converting the example above to this pattern:

```cpp
#include <mutex>

std::mutex mtx;

void some_thread () {
    // Creation of 'my_lock' acquires the lock
    std::lock_guard<std::mutex> my_lock(mtx);

    // some work while we have exclusive access
    ...

    // At the end of the function the 'my_lock' goes
    // out of scope and the lock is released. This
    // happens even if we exit the function because of
    // an exception, so it's very safe.
}
```

## Atomics

Mutexes are the most general way of protecting data, but C++11 has another means, which can be used for simple data types. This is the concept of an `atomic` variable. Changes to such a variable are guaranteed to happen in an atomic way - nothing in the program can see an intermediate or partial updated version of the variable.

This prevents the kind of race condition common in reductions, where two threads update a variable, but they both update the old version and so only one change really gets made.

Atomics are in the `atomic` header and the `std` namespace. The implementation is as a template, taking the data type on which on wishes to guarantee atomicity.

```cpp
#include <atomic>

std::atomic<int> atom_int;
std::atomic<double> atom_double;
```

The implementation of atomics varies from platform to platform, but will be lock free where the underlying hardware supports it (you can test this with `std::atomic::is_lock_free()`).

## Locks and Deadlocks

The flip side of a *data race* is a *deadlock*. This happens when two threads of the program hold locks that the other thread needs to proceed. This is important, because although a single mutex cannot itself cause a deadlock, when multiple mutexes are present they can. e.g., mutex `A` and mutex `B` will happy lock and unlock independently; however, if thread `1` holds mutex `A` and tries to lock mutex `B`, but thread `2` holds mutex `B` and it trying to lock mutex `A` then the program becomes deadlocked -- neither thread can proceed, but neither do they relinquish the lock they have.

There are a number of ways to avoid this situation:

1. Never hold more than one lock in the program: this is fine if your
   problem allows it, but in some cases multiple locks are
   unavoidable.

2. Acquire locks in a well defined order (in the example above, always
   acquire `A` then `B`). There are lock implementations called
   hierarchical locks, which help with this, by defining the order in
   which locks have to be acquired and throwing an exception if a
   thread tries to lock in the wrong order.

3. If an attempt to get a second (or subsequent) lock fails, force the
   release of all locks.

4. Acquire locks simultaneously.

There are many subtitles to efficient use of locks without destroying
the potential for concurrency, but in the last case the C++11 `mutex`
supports an *all or nothing* approach that can be very useful:

```cpp
#include <mutex>

std::mutex m;
std::mutex n;


void some_func() {
    std::lock(m, n);
    std::lock_guard<std::mutex> lock_m(m, std::adopt_lock);
    std::lock_guard<std::mutex> lock_n(n, std::adopt_lock);

    // Now I can work safely with both mutexes
```

Calling `std::lock(m,n)` locks both mutexes at the same time. Then lock guards are constructed, where the special argument `std::adopt_lock` tells the lock guard the mutex is already locked and it should just take ownership of that lock.

### Some General Points on Locks and Lock-free designs

Generally, whenever you are thinking to introduce a mutex into your code, you should consider if it's possible to avoid this by having a lock free design. This topic is advanced and we don't have time to explore it here, but in general *lock free* programming is going to scale much better than programming with locks (see references for a few introductory articles). Mutexes will limit your parallel performance and atomic variables are expensive to use.

If you do need to lock something in your program then try to hold the lock for the absolute minimum amount of time, i.e., accumulate some global change in local variables first, then acquire the lock to do an update and release the lock as quickly as possible. In this way many threads can calculate their "local" changes in parallel and then only contend for the lock briefly.

Likewise, in compound data objects, try to lock at the minimum granularity (*fine grained locking*) to reduce contention and keep as many of your threads busy for as much of the time as possible.

# Exercises

1. Take the program you wrote in the previous practical (Basic Thread Management) and get many threads to print to `std::cout`. You should find that the output is muddled up between the threads.
    1. Now use a `mutex` to protect the access to `stdout` and ensure that all threads print their *hello*.
    2. Use 50 threads and run the program a few times. Is the order of the output constant? Is this expected?

2. Start from the program `multithread-sum.cc`. Take a look at it and see how it calculates an *occupancy* for a 1-D detector in a serial
   and multi-threaded way.
    1. Compile and run the program a few times. Does it give the correct answer? Does it give a consistent answer?
    2. Use a `mutex` to fix the program.
    3. Can you use an `std::atomic` to fix the problem?

3. Taking the two fixed versions of the `multithread-sum.cc` from the previous exercise, investigate if there is a performance benefit to using `atomic` over `mutex`. (Increase the number of iterations if needed, to get good timing measurements.)
    1. For both solutions consider how to reduce the contention between threads to a minimum. (Try a few different strategies and take timing measurements.)

4. Run the program `deadlock.cc`.
    1. Can you see why it hangs?
    2. Fix it.
