Tips and References
===================

Tips
----

### Setup

This tutorial has been written and tested on *Scientific Linux 6*,
although it also works on *Mac OS 10.9* using gcc4.9 and TBB installed
from [MacPorts](https://www.macports.org/).

However, any reasonably modern C++11 compiler (gcc4.7 and up,
LLVM/Clang 3.3 and up) should be compatible, although you may need to
modify the makefiles. 

On SL6 it's important to setup the Redhat *devtoolset* to have access
to a modern version of gcc for all of the tutorial exercises.

To do this execute this command:

```bash
    $ scl enable devtoolset-2 bash
```

You can verify that all is well by asking for the gcc version, which
should be 4.8.2:

```bash
    $ g++ --version
    g++ (GCC) 4.8.2 20140120 (Red Hat 4.8.2-15)
	...
```

### C++11 in General

To compile concurrent C++11 programs you'll need two flags for g++:

* `-std=c++11` - Use the C++11 standard (`-std=c++1y` also works!)
* `-pthread` - Enable posix thread support, which is the underlying
      thread library used by libstdc++ on linux platforms

You might well find the `Makefile`
[here](https://github.com/graeme-a-stewart/cpp-concurrency/blob/master/src/cpp11/Makefile)
useful. It will compile any `.cc` file into an executable with the correct
compiler flags.

#### TBB

Intel Threaded Building Blocks has been compiled and installed into
`/usr/local` (specifically `/usr/local/include` for header files and
`/usr/local/lib64` for libraries). Thus you shouldn't need any special
extra setup to use is during the school.

Doxygen documentation for TBB is installed into
`/usr/local/share/tbb/html`, but using the
[Intel documentation](https://www.threadingbuildingblocks.org/) is
probably more convenient.

Tips
===

### Top and Threads

If you run `top -H` will see all running threads - this is useful for
a quick check that you have a multi-threaded program running! The `H`
key will cycle threads on/off once top is running.

So `top -H -u $USER` would probably be the most useful command to use.

### Timing

To measure any performance boost from threading you need to take
accurate timing measurements. The easiest way to do this is with the
`chrono` library (C++11).

```cpp
    #include <chrono>
    #include <iostream>

	int main() {
	    // Prologue stuff

	    auto start = std::chrono::high_resolution_clock::now();
        // Do work here
	    auto end = std::chrono::high_resolution_clock::now();

        auto duration = end - start;

	    std::cout << "That took: " << std::chrono::duration<float, std::milli> (duration).count() << "ms" << endl;

	   // Epilogue stuff
	}
```

Note that it's important to time the interesting bit of the program
only (which is why `time ./my_prog` isn't so useful).

**Caveat Emptor** If you run on *virtual machines* (e.g., GridKA
  School machines and `lxplus` at CERN) there is the possibility of
  interference and jitter. So, always take a few timing measurements,
  just to check that things are stable.

References
----------

### Generally Useful C++ Resources

* There are many good online C++ reference sites, e.g.,
  http://www.cplusplus.com/, (use the reference section, under
  multi-threading).
* Also worth noting is the official C++ website,
  http://www.isocpp.org/.

### Concurrency and Parallel Programming

* Baptiste Wicht has a nice tutorial on C++11 concurrency basics:
  http://baptiste-wicht.com/posts/2012/03/cpp11-concurrency-part1-start-threads.html
* Probably the best book on C++ concurrency is *C++ Concurrency in
  Action* by Anthony Williams, published by
  [Manning](http://www.manning.com/williams/). This book covers lock
  free programming in some detail.
* Jeff Pershing's
  [Introduction to Lock Free Programming](http://preshing.com/20120612/an-introduction-to-lock-free-programming/). 
* A great general introduction to the computer science of concurrency,
  which discusses a lot of the classic synchronisation problems in detail is *The
  Little Book of Semaphones* by Allen B. Downie. It's available here:
  http://greenteapress.com/semaphores/. (Essential reading to know
  what the *Sleeping Barber* problem is!)




### TBB

* The Intel website https://www.threadingbuildingblocks.org/ has much
  good reference and tutorial information.
* Intel's TBB
  [YouTube Channel](https://www.youtube.com/playlist?list=PLzwFYM4Q6gANxJmQDYXtyh6uRHO8JSY15).
* James Reinders, one of the authors of TBB wrote the O'Reilly book
  *Intel Threaded Building Blocks*. It is rather old now (2007)
  and although it has good discussions of concurrency in general, its TBB
  specific information is a bit out of date. 
