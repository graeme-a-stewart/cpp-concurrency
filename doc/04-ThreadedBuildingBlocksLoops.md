Threaded Building Blocks
========================

Introduction
------------

Intel Threaded Building Blocks (TBB) is an open source toolkit for
supporting scalable parallel programming in C++. It is a higher level
toolkit than C++11 threads and, if it's suitable for your problem,
it's almost certainly a better choice.

TBB supports common programming patterns for loops: `parallel_for` for
independent computations on arrays and containers, `parallel_reduce`
for a series of computations across an array and `parallel_scan` for
more general parallel prefix calculations.

However, TBB also includes additional support for multi-threaded
programming, including thread safe containers, a performant thread
safe memory allocator and timing primitives.

TBB also allows the construction of graphs describing the relationship
between different parts of a program's execution and the TBB scheduler
can exploit concurrency where different parts of the workflow are
independent.

### Using TBB

With most installations of TBB the only include file needed is
`tbb/tbb.h`. Depending on where TBB is installed you may need a
`-I/path/to/tbb/include` option to the compiler. (If you use only part
of TBB there may be a more specific header file that can be used.)

As TBB uses libraries, the `-ltbb` option to the linker is needed. On
linux `-lrt` is also required (POSIX real time extensions).

TBB objects live in the `tbb` namespace, which here we will give
explicitly. 


Parallel Loop Algorithms
------------------------

### Parallel For

One of the simplest parallel construct is one where we perform the
same operation on an array of values and there is no dependencies
between the operations. As a serial loop such an operation is just

```cpp
    for (size_t i=0; i<array_size; ++i) {
	    my_func(x[i]);
	}
```

The way that TBB turns such a serial loop into a parallel loop is to
express the operation as a *callable* object, which can take a special
TBB template class as an argument. e.g., we can write a class
supporting the `operator()` to do this:

```cpp
    #include "tbb/tbb.h"

    class ApplyFunc {
        double *const my_x;
    public:
        void operator()(const tbb::blocked_range<size_t>& r) const {
		    double *x = my_x;
			for(size_t i=r.begin(); i!=r.end(); ++i)
			    my_func(x[i]);
	    }
		ApplyFunc(double x[]):
		    my_x{x}
		{}
	};
```

Here the `()` operator will take the `tbb::blocked_range` reference,
which TBB uses to instruct a thread to operate on a certain chunk of
the `x` array. Note that using the class member variable `my_x` and
the local variable `x` is done simply for clarity and to make
comparison with the serial case easier. The body object must support a
copy constructor, so that TBB can make copies of it as needed; this is
why the `()` operator is declared `const` as it may not change this
body. Otherwise some local change might not be reflected in the
original object. (However, what the body points to, i.e., the contents
of `*my_x* can be changed.)

Once this class is constructed, the `tbb::parallel_for` function is
invoked, which will execute the operation in parallel:

```cpp
    #include "tbb/tbb.h"

    void ParallelApplyFunc(double x[], size_t n) {
	    tbb::parallel_for(blocked_range<size_t>(0, n), ApplyFunc(x));
	}
```

In this case `parallel_for` is instructed to run over the range
`[0,n)` on the `ApplyFunc(x)` class instance.

Note that `parallel_for` will take care of setting up the TBB thread
pool for us (in earlier versions of TBB, one needed to call
`tbb::task_scheduler_init`).

#### Using a Lambda

As the only requirements on the object passed to `tbb::parallel_for`
are that it's copyable and callable, we can use a C++11 lambda function
in place of writing a whole class. e.g.,

```cpp
    #include "tbb/tbb.h"

    void ParallelApplyFunc(double x[], size_t n) {
	    tbb::parallel_for(blocked_range<size_t>(0, n),
		                  [=](const blocked_range<size_t>& r) {
						      for(size_t i=r.begin(); i!=r.end(); ++i)
			                    my_func(x[i]);
	                      });
	}
```

Using the lamba which copies by value `[=]` satisfies all the criteria
and makes for a very succinct declaration.

Exercises
=========

1. Write a TBB parallel for loop which performs an operation on
   an array of 10^6 doubles. e.g., take the `log` of the values. Also
   write a serial version of the same operation.
	1. Is the TBB version faster?
	2. What happens if you change the array size by an order of magnitude (up and down)?


