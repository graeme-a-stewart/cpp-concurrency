# Startup, Tips and References

## Startup

### Respository

All of the documentation for this tutorial and the example
code is stored in 
[GitHub](https://github.com/graeme-a-stewart/cpp-concurrency).

Just clone the repository in the normal way, e.g.,:

```sh
git clone https://github.com/graeme-a-stewart/cpp-concurrency.git
```

### Setup and Running

The current version of this tutorial has been developed on 
OS X 10.12 (with Threaded Building Blocks and Boost installed
using [homebrew](https://brew.sh/) and also tested with Fedora 26
(install TBB and Boost runtime and development RPMs).

Most modern Linux distributions should work fine, as long as you
have an up to date C++ compiler (C++11 support is absolutely
needed) with TBB and Boost installed.

A Dockerfile that sets up Fedora 26 correctly is in the repository
(`docker/cpp-concurrency-fedora`); you can also use the docker image
`graemeastewart/cpp-concurrency-fedora` from Docker Hub. If you use the
docker image remember to use a bind mount to a persistent
filesystem for your work!

```sh
docker run -it -v $HOME:/workspace graemeastewart/cpp-concurrency-fedora
```

Both the GridKA VMs and the docker container have a variety of 
editors installed: `nano`, `jed`, `emacs`, `geany`, `vim`.
The advantage of using the docker container is that you can use
an editor or IDE on your host. To use
an X editors, like geany, use the `-X` option to ssh (VM) or pass in the correct
`DISPLAY` setting using `-e` (docker). e.g., for docker
on the Mac:

```sh
docker run -it -e DISPLAY=$(ipconfig getifaddr en0):0 \
-v $HOME:/workspace graemeastewart/cpp-concurrency-fedora
```

(you may need to run `xhost +$(ipconfig getifaddr en0`) on the Mac host;
make sure that XQuartz allows network clients to connect in
Preferences->Security).

## Tips

### C++11 in General

To compile concurrent C++11 programs you'll need some compiler and linker flags:

* `-std=c++11` - Use the C++11 standard (`-std=c++14`
  also works, if your compiler supports that)
* `-lpthread` - Enable posix thread support, which is the underlying thread library used by libstdc++ on linux platforms

You might well find the `Makefile` [here](https://github.com/graeme-a-stewart/cpp-concurrency/blob/master/src/cpp11/Makefile)
useful. It will compile any `.cc` file into a like named executable with the correct compiler flags.

#### TBB

Depending on how TBB has been installed in your environment you make need to add
correct include paths and link options to your compiler command. In the school
VMs and the Docker container no special options are needed beyond `-ltbb`.

[Intel documentation](https://www.threadingbuildingblocks.org/) for TBB is
very convenient.

### CMake

If you know CMake, there is a `CMakeLists.txt` file provided that will
compile all of the tutorial examples and solutions. As there is a pre-existing `Makefile` 
in the source directory you must do an out of source build (which is best practice anyway).

To prepare to run CMake you will need the `FindTBB` module, which is
available as a submodule of this repo. To get it do:

```sh
git submodule init
git submodule update
```

Then using CMake is very easy:

1. Run `cmake ../path/to/the/source` in the place you want to build the examples
2. Then just use `make` as normal
 
If you add your own programs you'll need to change the `CMakeLists.txt` file.

## Tips

### Top and Threads

If you run `top -H` will see all running threads -- this is useful for a quick check that you have a multi-threaded program running! The `H` key will cycle threads on/off once top is running.

So `top -H -u $USER` would probably be the most useful command to use.

### Timing

To measure any performance boost from threading you need to take accurate timing measurements.
The easiest way to do this is with the `chrono` library from C++11.

```cpp
#include <chrono>
#include <iostream>

int main() {
    // Prologue stuff

    auto start = std::chrono::high_resolution_clock::now();
    // Do work here
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = end - start;

    std::cout << "That took: " << std::chrono::duration<float, std::milli> (duration).count() << " ms" << endl;

   // Epilogue stuff
}
```

Note that it's important to time the interesting bit of the program only (which is why `time ./my_prog` isn't so useful).

**Caveat Emptor** If you run on *virtual machines* (e.g., GridKA School machines or `lxplus` at CERN) 
there is the possibility of interference and jitter. So, always take a few timing measurements, 
just to check that things are stable.

## References

### Generally Useful C++ Resources

* There are many good online C++ reference sites, e.g., http://www.cplusplus.com/, (use the reference section, under
  multi-threading) and http://en.cppreference.com/ (Thread Support Library section).
* Also worth noting is the official C++ website, http://www.isocpp.org/.

### Concurrency and Parallel Programming

* Baptiste Wicht has a nice tutorial on C++11 concurrency basics: http://baptiste-wicht.com/posts/2012/03/cpp11-concurrency-part1-start-threads.html
* Probably the best book on C++ concurrency is *C++ Concurrency in
  Action* by Anthony Williams, published by
  [Manning](http://www.manning.com/williams/). This book covers lock
  free programming in some detail.
* Nicolai M. Josuttis' *The C++ Standard Library: A Tutorial and Reference, Second Edition*
  has a good chapter on C++11 concurrency. 
* Jeff Pershing's [Introduction to Lock Free Programming](http://preshing.com/20120612/an-introduction-to-lock-free-programming/).
* A great general introduction to the computer science of concurrency,
  which discusses a lot of the classic synchronisation problems in
  detail is *The Little Book of Semaphones* by Allen B. Downie. It's
  available here: http://greenteapress.com/semaphores/. (Essential
  reading to know what the *Sleeping Barber* problem is!)
* A highly practical example of concurrent data access is the
  Read-Copy-Update pattern, https://lwn.net/Articles/573424/.
* A modern book on parallel programming patterns and exploiting
  concurrency is *Structured Parallel Programming* by Michael McCool,
  James Reinders and Arch Robison (Elsevier, 2012).

### TBB

* The Intel website https://www.threadingbuildingblocks.org/ has much
  good reference and tutorial information.
* Intel's TBB [YouTube Channel](https://www.youtube.com/playlist?list=PLzwFYM4Q6gANxJmQDYXtyh6uRHO8JSY15).
* James Reinders, one of the authors of TBB, wrote the O'Reilly book
  *Intel Threaded Building Blocks*. It is rather old now (2007) and
  although it has good discussions of concurrency in general, its TBB
  specific information is a bit out of date.
