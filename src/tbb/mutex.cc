/*
illustration of thread protection of shared variable using TBB mutex
*/

#include <iostream>
#include <tbb/mutex.h>

int count;
tbb::mutex countMutex;

int IncrementCount(){

    int result;
    // Add a TBB mutex.
    // Lock the mutex (lock() implements ANNOTATE_LOCK_ACQUIRE()).
    countMutex.lock();
    // Access the shared object (save the result until after unlocking).
    result = count++;
    // Unlock the mutex (unlock() implements ANNOTATE_LOCK_RELEASE()).
    countMutex.unlock();
    return result;

}

int main(){

    for (int i = 0; i < i; ++i){
        IncrementCount();
    }
    return 0;

}
