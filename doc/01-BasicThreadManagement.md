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

Core functionality for threads are found in the `thread` 
[header](http://www.cplusplus.com/reference/thread/). So to
begin we need to ensure we have this included:

```cpp
	#include <thread>
```

As expected, thread support is found in the `std` namespace. (In this
documentation usually we'll give this namespace explicity, but it's
quite common for tutorials to miss this assuming a 
`using namespace std;`.)

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

### Thread Arguments ###

To pass an argument to the thread when you start it, just add the
arguments as extra parameters when you create the thread. e.g.,

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

(An alternative way is to use `std::bind` to create a temporary
function with the arguments already bound to the parameters you want to use,
i.e., `std::thread my_second_thread(std::bind(say_hello_msg, 1));`.)

### Warning ###

:boom: You need to take a little care when passing arguments. `std::thread`
will copy arguments before starting a thread and this can lead to some
subtle problems.

#### Copied data is not the original ####

```cpp
	void update_widget(widget wdgt) {
		...
    }
    
    void widget_updates(widget widget_list[]) {
		widget real_wdgt;
		std::vector<std::thread> widget_threads;
		
		// Intercept widget update requests
		for (auto& real_wdgt: widget_list) {
			if (request_update(real_wdgt))
				widget_threads.push_back(std::thread(update_widget, real_wdgt));
		
		// Do some other things
		for (auto& each_thread: widget_threads)
			each_thread.join();
	}
```

In this case `std::thread` copies `real_widget` before calling
`update_widget`. So the side effects of calling `update_widget`
are never seen in `real_widget`, but are lost in the empeheral 
copy.

Overcome this problem by calling `std::ref` on `real_wdgt` to
create a reference, which can be copied and still refer to the
original object: `std::thread(update_widget, std::ref(real_wdgt)`.

#### Data going out of scope ####


```cpp
	void do_work(std::vector<objects>& stuff) {
		...
    }
    
    void thread_spawner() {
		std::vector<objects> my_stuff{};
		// Create various objects
		....
		
		auto my_thread = std::thread(do_work, stuff);
		my_thread.detach();
	}
```

In this case `std::thread` will copy a reference to `my_stuff`
and pass this in to the thread executing `do_work`. However, as
the thread detaches and `thread_spawner` will very likely exit
before `do_work`, the thread will be left with an invalid reference
and _undefined behaviour_ will result.
