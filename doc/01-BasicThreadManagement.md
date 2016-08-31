Title: Basic Thread Management
Author: Graeme A Stewart

# Basic Thread Management

## Introduction

C++11 introduced support in the core C++ language for concurrency. This makes it one of the most useful toolkits for
concurrency and multi-threading as any compliant compiler will support its constructs, independent of the underlying platform (e.g., on unix systems `pthreads` is used to manage the threads, whereas on Windows the Win32 API will be used).

As C++ develops we can expect that this concurrency support will become richer and more useful, e.g., task
based parallelism and vectorisation support are actively being considered for forthcoming versions of C++. 

The C++ threading libraries are also very useful for introducing some core concepts in concurrency, 
which is one of the primary reasons for looking at them here.

## Starting Threads

Core functionality for threads are found in the `thread` [header](http://www.cplusplus.com/reference/thread/). So to begin we need to ensure we have this included:

```cpp
    #include <thread>
```

As expected, thread support is found in the `std` namespace. (In this documentation usually we'll give this namespace explicity, but it's quite common for tutorials to omit this, assuming a  `using namespace std;`.)

To start a thread we should give it something to do[^1], so we pass the name of a function that we wish to call in the thread as the argument
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

Note that the execution of `my_first_thread` happens immediately we create it. We then use the `std::thread::join()` method to ask the current (main) thread to wait for the other thread to finish. Then we can exit.

The alternative to using `join` when a thread is created to use `detach`, which will give control of the thread to the C++ runtime (which will then clean up the thread when it exits).

Note that you must use `join` or `detach` when you have created a thread before the creating function exits, otherwise `std::terminate` is
called -- i.e., you have to explicitly state if you are keeping control of the thread or passing it to the runtime.

It's also illegal to `join` or `detach` a thread which has already done one of these things. To avoid this use `std::thread::joinable()`, which will return `true` only if the thread can be joined (or detached - the condition is identical).

Note that to protect against exceptions, one can use a *thread guard* pattern, where the thread is a resource that is joined (if `joinable`) in its destructor (this is the Resource Acquisition Is Initialisation, RAII, pattern).

[^1]: You can construct an `std::thread` object that doesn't represent an executing thread object by giving no arguments to the constructor (`std::thread th()`). This is useful if you want to move an existing thread to a new object.

### Thread Arguments

To pass an argument to the thread when you start it, just add the arguments as extra parameters when you create the thread. e.g.,

```cpp
    void say_hello_msg(int my_id) {
        std::cout << "Hello from thread number " << my_id << std::endl;
    }

    int main() {
        std::thread my_second_thread(say_hello_msg, 1);

        my_second_thread.join();

        return 0;
    }
```

Which should give:

```
    $ ./my-second-thread
    Hello from thread number 1
```

So, the _callable_ first argument is invoked with the second and subsequent arguments passed to it.

An alternative way is to use `std::bind` to create a temporary function with the arguments already bound to the parameters you want to use,
e.g.,

```cpp
auto hello_with_one = std::bind(say_hello_msg, 1);
std::thread my_second_thread(hello_with_one);
```

#### Things that can go wrong passing arguments

You need to take a little care when passing arguments. `std::thread`
will _copy_ arguments before starting a thread and this can lead to some
subtle problems.

##### Copied data is not the original

```cpp
void update_widget(widget& wdgt) {
...
}

void widget_updates(widget widget_list[]) {
    std::vector<std::thread> widget_threads;
    
    // Intercept widget update requests
    for (auto& real_wdgt: widget_list) {
        if (request_update(real_wdgt))
            widget_threads.push_back(std::thread(update_widget, real_wdgt));
    
    // Now wait for all widgets to be updated
    for (auto& each_thread: widget_threads)
        each_thread.join();
}
```

In this case `std::thread` copies `real_widget` before calling `update_widget` (let's assume it has a copy constructor). So the side effects of calling `update_widget` are never seen in `real_widget`, but are lost in this empeheral copy.

Overcome this problem by calling `std::ref` on `real_wdgt` to create a reference, which can be copied and still refer to the original object: `std::thread(update_widget, std::ref(real_wdgt))`.

##### Input data going out of scope

```cpp
void do_work(std::vector<objects>& stuff) {
    ...
}

void thread_spawner() {
    std::vector<objects> my_stuff{};
    // Create various objects
    ....
    
    auto my_thread = std::thread(do_work, std::ref(my_stuff));
    my_thread.detach();
}
```

In this case `std::thread` will copy a reference to `my_stuff`
and pass this in to the thread executing `do_work`. However, as
the thread detaches and `thread_spawner` will very likely exit
before `do_work`, the thread will be left with an invalid reference
to `my_stuff` and _undefined behaviour_ will result.

Note that a similar problem can occur when arguments need to be
converted from one type to another as the conversion only happens in
the context of the newly spawned thread, so beware of constructing
objects from a pointer to local data.

### Class Methods Called from Threads

If you want to call a class method from a thread, then you need to
give a function pointer using the `&class::method` syntax. In addition
the first parameter to a class method has to be the magic pointer
`this` (which always points to **this** instance).

```cpp
class foo {
    ....
public:
    void bar(double x) { ... }

    void spawn_bar_thread() {
        auto my_thread = std::thread(&foo::bar, this, 0.0);
        ...
    }

};
```

## Thread Identity

Threads all have a unique identifier accessed via the `get_id()` method of `std::thread`. For a thread to access its own identity, one can use the `std::this_thread` namespace.

```cpp
    #include <thread>

    void hello_n(int i) {
        std::cout << "hello, world (from thread number " << i
            << "; thread id" << std::this_thread::get_id() << ")" << std::endl;
    }
```

## Hardware Concurrency

The `std::thread` library also has a method to get the number of available hardware threads on a machine, `std::thread::hardware_concurrency`. This returns an unsigned number that estimates the number of concurrent threads that the hardware supports. Note that there is no guarantee that this is exact, but it can provide a useful guide to how much work a hardware platform might be expected to do (e.g., on Intel chips with hyperthreading, each hyperthreaded core can be counted, as well as each physical core).

# Exercises

1. Write a C++ program that spawns a thread and writes a message to `stdout` from it.
    1. Now spawn multiple threads and get them all to say hello.

1. Write a C++ program that spawns multiple threads, where each one executes a program with arguments.
     1. Modify the program so that each thread states its thread id.

1. Look at the programs `thread-arguments-problems.cc` and `thread-arguments-problems2.cc`.
     1. Try to understand why these programs are not working correctly.
     1. Fix them.

1. What's the hardware concurrency value on the test machine?
    1. If you have a suitable setup on your laptop to run C++11,
       what's the value there?
