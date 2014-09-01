Tips and References
===================

Tips
----

### Setup

It's important to setup the Redhat *devtoolset* to have access to a
modern version of gcc for all of the tutorial exercises.

To do this login and execute this command:

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

* `-std=c++11` - Use the new C++11 standards
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

Documentation for TBB is installed into `/usr/local/share/tbb/html`,
but using the
[Intel documentation](https://www.threadingbuildingblocks.org/) is
probably more convenient.


References
----------

### C++11 Concurrency

* There are many good online C++ reference sites, e.g.,
  http://www.cplusplus.com/, (use the reference section, under
  multi-threading).
* Also worth noting is the official C++ website,
  http://www.isocpp.org/.
* Baptiste Wicht has a nice tutorial on C++11 concurrency basics:
  http://baptiste-wicht.com/posts/2012/03/cpp11-concurrency-part1-start-threads.html
* A great general introduction to the computer science of concurrency,
  which discusses a lot of the generic synchronisation problems in detail is *The
  Little Book of Semaphones* by Allen B. Downie. It's available here:
  http://greenteapress.com/semaphores/. (Essential reading to know
  what the *Sleeping Barber* problem is!)
* Probably the best book on C++ concurrency is *C++ Concurrency in
  Action* by Anthony Williams, published by [Manning](http://www.manning.com/williams/).



### TBB

* The Intel website https://www.threadingbuildingblocks.org/ has much
  good reference and tutorial information.
* James Reinders, one of the authors of TBB wrote the O'Reilly book
  *Intel Threaded Building Blocks*. It is rather old now (2007)
  and although it has good discussions of concurrency in general, its TBB
  specific information is a bit out of date. 
