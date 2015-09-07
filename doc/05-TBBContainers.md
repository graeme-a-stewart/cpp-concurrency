# Threaded Building Blocks Containers

## Introduction

One of the problems faced by programmers in a parallel environment is that Standard Template Library containers are not generally thread safe. Concurrent writes to STL containers will often corrupt them. So, usually, access to such containers has to be protected by a mutex. However, this makes the container a bottleneck and may slow down a threaded application.

Threaded Building Blocks offers another way, which is to introduce concurrency friendly containers. These containers support as much as possible of the STL interface, but not all STL operations are present as some of these are inherently difficult to make safely concurrent.

There are two strategies to make such containers:

* Use lock-free techniques, which correct for any interference between threads automatically.

* Use fine grained locking, where only the part of the container which is being touched is wrapped in a mutex.

Not all is sweetness and light, however. Because of the extra protections needed by concurrent containers, they will often be a bit slower than STL containers in serial cases. So it's important to experiment to see whether the higher concurrency allowed by TBB containers overcomes the higher latencies.

## Concurrent vector

You won't be surprised that a `concurrent_vector<T>` is a template class that allows dynamic growth of an array of type `T`, just like its STL counterpart. What's nice about the concurrent vector is that it can be grown safely while other threads are accessing it, including growing it themselves.

As sometimes we want to make sure that a group of elements added by a thread are contiguous in the vector, as well as the usual `push_back()` method there are the methods `grow_by(n)` that adds `n` elements and `grow_to_at_least(n)` that ensures that the vector has space for at least `n` elements. New elements are initialised with `T()`. `grow_by` return an iterator to the first element that was
added.

e.g., appending `n` elements to a `concurrent_vector<double>` is done like this:

```cpp
add_to_concurrent_vector(concurrent_vector<double> v, double new_stuff[], size_t n) {
    auto my_iterator = v.grow_by(n);
    for (size_t i=0; i<n; ++i) 
        *my_iterator++ = new_stuff[i];
}
```

There are a few things to be careful of with `concurrent_vector`, namely:

* Clearing or destroying the vector and accessing it is *unsafe*.

* `size()` returns the size of all elements, even those being constructed, so accessing those elements might be problematic.

* Unlike an `std::vector`, elements in a concurrent vector are *not* guaranteed to be contiguous in memory.

## Other Concurrent Containers

TBB supports many other concurrent containers, including hash maps and queues. We won't cover them here, but having practiced with the concurrent vector you should find them pretty easy to use.

# Exercises

1. Write a piece of code that appends multiple elements to a `concurrent_vector` in different threads at the same time.
    1. You might want to use a `typedef` to be able to switch back to a normal `std::vector` and see what happens if you do that without a mutex.
    2. *Tip* You can run multiple threads just using some `std::thread`s or you can use this simple trick to get TBB to run some threads for you:

```cpp
const int size = 10000;
tbb::parallel_for(0, size, 1, [=](int i){my_function(i);});
```

2. Based on the previous example, write a *filler* that selects random numbers and writes them concurrently into a `concurrent_vector`. Then write a *filter* that only selects some of the random numbers and puts them into an output `concurrent_vector`.
