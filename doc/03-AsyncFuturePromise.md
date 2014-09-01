# Async Tasks, Futures and Promises


## Introduction


Running threads directly from `std::thread` does have some drawbacks,
particularly because there's no direct way to get the return value of
the function the thread executed. Using messages or shared objects
isn't a great way of doing this either, but the standard library
offers some very convenient ways to manage this.

## Async Tasks and Futures 

An `async` task is very like launching a thread - it has the same
syntax, giving the name of the function to call and any arguments it
shoudl take. However, the return value from an `async` task is a
special class template, a `future`, in which the return value will
only be placed once the thread has executed and returned.

So, we just launch a thread as an `std::async` object and assign the
return value to the `std::future` templated class. E.g.,


```cpp
    #include <future>

    double some_expensive_operation(std::vector<thing>& stuff) {
        // Do expensive things to stuff
        ...

        return result;
    }


    int main() { auto my_stuff = initialise_stuff();

        std::future<double> res = std::async(some_expensive_operation, std::ref(my_stuff));

        // do some other things

        double the_answer = res.get();
        ...
```

You can see that the value of the variable is accessed with the `get`
method of the `future`.


