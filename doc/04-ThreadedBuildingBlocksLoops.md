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
`tbb::task_scheduler_init`, which is still available if you need
to setup the TBB thread pool with special options).

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
needed by `parallel_for` and makes for a very succinct declaration.

### Parallel Reduce

A `parallel_for` is great when operations really are
independent. However, sometimes we need to do some computation on the
input data that produces an aggregated result that depends on all the
input data. If the computation can be broken down into pieces we can
still parallelise this operation, which is a *reduction*.

Here's a simple example: if I need to sum up 12 numbers I can do it
like this

```
    (((((((((1+2)+3)+4)+5)+6)+7)+8)+9)+10)
```

which is sequential. But I could also do it like this:

```
    ((1+2) + (3+4)) + ((5+6) + ((7+8) + (9+10)))
```

where it's now much easier to see that the calculation can be
parallelised.

Clearly there are some extra steps here, where one needs to *join* the
results of partial calculations into an aggregated result. So, one
has to teach TBB how to do this to be able to make a reduction
successfully. Likewise TBB is going to need to know how to *split* the
original calculation into pieces so that it can fill all the hardware
threads and exploit the potential of the machine.

Here's a simple example, where the reduction is just a sum of all values in
an array:

```cpp
    #include <tbb/tbb.h>

    double serial_sum(double x[], size_t n) {
	    double the_answer = 0.0;
		for (size_t i=0; i<n; ++i)
		    the_answer += x[i];
		return the_answer;
	}

	class parallel_sum {
	    double *my_x;
	public:
		double the_answer;
		void operator()(const tbb::blocked_range<size_t>& r) {
		    double *x=my_x;
			for(i=r.begin(); i!=r.end(); ++i)
			    the_answer += x[i];
	    }

        parallel_sum(parallel_sum& a, tbb::split):
		  my_x{a.x}, the_answer{0.0} {};

        void join(const parallel_sum& b) {
		    the_answer += b.the_answer;
		}

        parallel_sum(double x[]):
		  my_x{x} {};
	};
```

Notice the two new methods that were needed

* A special constructor that takes a reference to an existing
`parallel_sum` instance and a special dummy parameter
`tbb::split`. This is the splitting constructor that TBB uses to
generate new parts of the reduction that will run on other
threads. (The dummy variable distinguishes this method from the
class's copy constructor.)

* A `join` method that tells TBB how to combine the results from one
  fragment of the reduction with another (in this case, just adding up
  the partial sums).

Once the class is ready, we invoke the reduction by calling
`parallel_reduce`:

```cpp
    double Apply_parallel_sum(const double x[], size_t n) {
	    parallel_sum ps(x);
		parallel_reduce(tbb::blocked_range<size_t>(0, n), ps);
		return ps.the_answer;
	}
```

Note that because of the extra methods that are needed for
`parallel_reduce` it's not possible to use a lambda.

Blocked Range and Grain Size
-

When we used the ``blocked_range`` above we only specified the start
and end values of the iteration. TBB tries to discover itself what the
optimal way to divide the problem is. This division is called the
*grain size*. The automatic grain size determination is now pretty
good in TBB, but it is possible to pass a manual grain size that
might eke out some more performance:

```cpp
	size_t grain_size = 8;
    tbb::blocked_range<size_t>(0, n, grain_size);
```

However, beware that the best grain size on one machine might not be
the best on another, so use this option carefully.

TBB Timing Goodies
---------

TBB includes some utilities for taking timing measurements, which can
be useful, although they can largely be replaced by the standard `chrono`
library now. However, if you want to use them here's how:

```cpp
    #include <tbb/tick_count.h>

    tbb::tick_count t0 = tbb::tick_count::now();
    // do work here
	...
    tbb::tick_count t1 = tbb::tick_count::now();
    auto time_interval = t1-t0;
    std::cout << "Work took " << time_interval.seconds() << "s" << std::endl;
```


Exercises
=========

1. Write a TBB parallel for loop using a class which performs an operation on an
   array of 10^6 doubles. Take the `log` of the input as the work to
   be done. Also write a serial version of the same operation.
	1. Is the TBB version faster?
	2. Investigate the effect of changing the array size - use steps of
       10 and see if you can find out the best speedup that TBB can
       give you and also the worst (i.e., what's the overhead of using
       TBB).

2. Repeat the first part of the above exercise using a lambda.

3. Write a TBB parallel reduction that calculates a simple reduced
   value from 10^6 doubles (sum or product will be fine). Also write a
   serial version.
     1. Investigate the scaling properties as above.
	 2. Do you understand any differences from the first exercise?

4. Using a `blocked_range2d`, write a `parallel_for` loop that
   calculates the set of points in the Mandelbrot set in the `(x,y)`
   range `[-2,+2]`.

	Recall that the Mandelbrot set is the set of points (r0) where:

	z_i = z_(i-1)^2 + r0; z0 = 0

	remains finite.
	


