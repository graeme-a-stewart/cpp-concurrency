TBB Tasks allow parallelization similar to the parallel loops, 
but tasks give you direct control over the grain size and allow 
parallelization of independent tasks without requiring a loop. 
As usual, with more control comes more power, with more power 
comes more responsibility (i.e. if you make lots of tiny tasks, 
TBB will abide and performance will suffer, ref parallel_for_each). 
The documentation states:

"A rule of thumb is that grainsize iterations of operator() should take at least 10,000-100,000 instructions to execute."

e.g. 200 sin() calls. 

```
    #include "tbb/task_group.h"
    int tbbFib(int n) {
        if( n<2 ) {
            return n;
        } else {
            int x, y;
            tbb::task_group g;
            g.run([&]{x=Fib(n-1);}); // spawn a task
            g.run([&]{y=Fib(n-2);}); // spawn another task
            g.wait();                // wait for both tasks to complete
            return x+y;
        }
    }
```

This is already an advanced example as it recursively starts dependent 
tasks that have to wait on each other. It also uses tasks nested in other 
tasks which doesn't require brain work on the developer's side. But 
performance wise it's a bad example: a single addition is not really an 
appropriate task size. Changing this to doing more work per task leads to significantly improved performance:
```
int serialFib(int n){ 
  if (n< 2) {
    return n;
  }
  else {
    return serialFib(n-1)+serialFib(n-2);
  }
}

int tbbFib(int n) {
  if( n<20){
    return serialFib(n);
  }
  ...
```

Normally I use tasks for heavy independent problems that do not come in a 
loop:

```
#include "tbb/task_group.h"
#include <vector>

int main(){
 
  ReturnType1 result1;
  ReturnType2 result2;
  ReturnType3 result3;
  
  tbb::task_group g;
  g.run([&]{result1 = hardwork1();});
  g.run([&]{result2 = hardwork2();});
  g.run([&]{result3 = hardwork3();});

  g.wait();

  return 0;
}
```

The hardwork functions can contain other parallelizations, e.g. parallel_for
of internal loops as shown before. Load will be distributed automatically to
tbb's thread pool, the nested parallelism has no side effects. 

