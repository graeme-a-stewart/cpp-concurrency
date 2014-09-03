/*
illustration of thread protection of shared variable using TBB scoped_lock
*/

#include <tbb/mutex.h>

int count;
tbb::mutex countMutex;

int IncrementCount(){

    int result;
    // Create a scope in order to destroy objects on leaving the scope.
    {
        // Add a TBB scoped lock at the location of ANNOTATE_LOCK annotations.
        // The creation of the scoped lock object implements
        // ANNOTATE_LOCK_ACQUIRE() on the specified object.
        tbb::mutex::scoped_lock lock(countMutex);
        result = count++;
        // The destruction of the scoped lock object implements
        // ANNOTATE_LOCK_RELEASE() on leaving the scope. 
    // Leave the scope, destroying the scoped lock object and, so, releasing
    // the mutex lock.
    }
    return result;

}

int main(){

    for (int i = 0; i < i; ++i){
        IncrementCount();
    }
    return 0;

}
