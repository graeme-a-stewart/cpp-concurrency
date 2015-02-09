TBB Tasks allow parallelization similar to the parallel loops, 
but tasks give you direct control over the grain size and allow 
parallelization of independent tasks without requiring a loop. 
As usual, with more control comes more power, with more power 
comes more responsibility (i.e. if you make lots of tiny tasks, 
TBB will abide and performance will suffer, ref parallel_for_each). 
A rule of thumb is that a task should have at least 10000 micro 
operations such as multiplications or a similar load, e.g. 200 sin()
calls. 

```
    #include "tbb/task_group.h"
    int fib(int n) {
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
appropriate task size. I use tasks for few heavy independent problems 
that do not come in a loop. These then can contain other parallelizations.

```
#include "tbb/task_group.h"
#include <vector>
#include <iostream>


class SpacePoint{
  double x;
  double y;
  double z;
};

std::auto_ptr<SpacePoint> extrapolate(SpacePoint a, SpacePoint b){ 
  std::auto_ptr<SpacePoint> c( new SpacePoint());
  //extrapolate a and b to c here
  return c;
}

std::auto_ptr< std::vector< std::auto_ptr<SpacePoint> > > 
                 findSeedVertices(std::vector<SpacePoint>& layer1, std::vector<SpacePoint>& layer2){

  std::auto_ptr< std::vector< std::auto_ptr<SpacePoint> > > vertices( new std::vector< std::auto_ptr<SpacePoint> >());
  for (auto sp1 : layer1){
    for (auto sp2 : layer2){
      vertices->push_back(extrapolate(sp1,sp2));
    }   
  }
  return vertices;
}

int main(){

  std::vector<SpacePoint> pixelLayer0(5000);
  std::vector<SpacePoint> pixelLayer1(5000);
  std::vector<SpacePoint> pixelLayer2(5000);

  tbb::task_group g;
  std::auto_ptr< std::vector< std::auto_ptr<SpacePoint> > > verticesL0L1;
  g.run([&]{verticesL0L1 = findSeedVertices(pixelLayer0, pixelLayer1);});

  std::auto_ptr< std::vector< std::auto_ptr<SpacePoint> > > verticesL0L2;
  g.run([&]{verticesL0L2 = findSeedVertices(pixelLayer0, pixelLayer2);});

  std::auto_ptr< std::vector< std::auto_ptr<SpacePoint> > > verticesL1L2;
  g.run([&]{verticesL1L2 = findSeedVertices(pixelLayer1, pixelLayer2);});

  g.wait();

  std::cout << "found Combinations: " << verticesL0L1->size()+verticesL1L2->size()+verticesL0L2->size() << std::endl;
  return 0;
}
```

Another parallelization opportunity would be the double loop over the two 
vectors. This is done simply by changing the loop to a parallel_for as shown 
before. Load will be distributed automatically, the nesting has no side 
effects. 


Side note A: I'm unable to create a memory leak when using auto_ptr!

Side note B: The (single threaded) destruction of the 75 million space
points takes longer than the algorithm. Try this at home but don't
try this in production code!
