Title: Basic Thread Management
Author: Graeme A Stewart

Basic Thread Management
=======================

Introduction
------------

C++11 introduced support in the core C++ language for
concurrency. This makes it one of the most useful toolkits for
concurrency and multi-threading as any compliant compiler will support
its constructs, independent of the underlying platform (e.g., on 
unix systems `pthreads` is used to manage the threads, whereas on
Windows the Win32 API will be used).

As C++ develops we can expect that this concurrency support will
become richer and more useful.

Starting Threads
----------------

Core functionality for threads are found in the `thread` [header](http://www.cplusplus.com/reference/thread/). So to
begin we need to ensure we have this included:

```cpp
#include <thread>
```

As expected, thread support is found in the `std` namespace. (In this
documentation usually we'll give this namespace explicity.)

To start a thread we need to give it something to do, so we pass the
name of a function that we wish to call in the thread as the argument
to `std::thread`. e.g.,

```cpp
void say_hello() {
  std::cout << "Hello from a thread" << std::endl;
}

int main() {
  std::thread my_first_thread(say_hello);

  my_first_thread.join();

  return 0;
}
```

Note that the execution of `my_first_thread` happens immediately we
create it. We then use the `std::thread.join()` method to ask the
program to wait for the thread to finish. Then we can exit.

The alternative to using `join` when a thread is created to use `detach`,
which will give control of the thread to the C++ runtime.

Note that if you do not use `join` or `detach` when you have created a
thread and the creating function exits, then `std::terminate` is
called -- so you have to explicitly state if you are keeping control
of the thread or passing it to the runtime.

Thread Arguments
-

To pass an argument to the thread when you start it, just add the
arguments as extra parameters when you create the thread. e.g.,

``` cpp
void say_hello_msg(int my_id) {
  std::cout << "Hello from thread number " << my_id << std::endl;
}

int main() {
  std::thread my_second_thread(say_hello_msg, 1);

  my_second_thread.join();

  return 0;
}
```

### Warning ###

You need to take a little care when passing arguments. `std::thread`
will copy arguments before starting a thread and this can lead to some
subtle problems.



