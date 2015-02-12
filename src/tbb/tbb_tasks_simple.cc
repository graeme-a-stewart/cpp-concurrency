#include "tbb/task_group.h"
#include <vector>
#include <iostream>


class SpacePoint{
  double x;
  double y;
  double z;
};


// dynamic memory allocation is memory bound, not cpu bound. 
// But it's easy to imagine something useful and probably cpu bound could be done here
std::unique_ptr<SpacePoint> extrapolate(SpacePoint a, SpacePoint b){ 
  std::unique_ptr<SpacePoint> c( new SpacePoint());
  //extrapolate a and b to c here
  return c;
}


// do try this at home but DO NOT create 75 million new'ed elements in production code :)
std::unique_ptr< std::vector< std::unique_ptr<SpacePoint> > > 
                 findSeedVertices(std::vector<SpacePoint>& layer1, std::vector<SpacePoint>& layer2){

  std::unique_ptr< std::vector< std::unique_ptr<SpacePoint> > > vertices( new std::vector< std::unique_ptr<SpacePoint> >());
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
  std::unique_ptr< std::vector< std::unique_ptr<SpacePoint> > > verticesL0L1;
  g.run([&]{verticesL0L1 = findSeedVertices(pixelLayer0, pixelLayer1);});

  std::unique_ptr< std::vector< std::unique_ptr<SpacePoint> > > verticesL0L2;
  g.run([&]{verticesL0L2 = findSeedVertices(pixelLayer0, pixelLayer2);});

  std::unique_ptr< std::vector< std::unique_ptr<SpacePoint> > > verticesL1L2;
  g.run([&]{verticesL1L2 = findSeedVertices(pixelLayer1, pixelLayer2);});

  g.wait();

  std::cout << "found Combinations: " << verticesL0L1->size()+verticesL1L2->size()+verticesL0L2->size() << std::endl;

  //side note: The auto pointers are destructed here. 
  //I cannot create a memory leak with auto pointers. 
  //Use auto pointers when passing pointers to somewhere outside of your control.
  return 0;
}
