#include "tbb/tbb.h"
#include <iostream>
#include <numeric>

double ParallelSum(std::vector<double> vec){
  return tbb::parallel_reduce(tbb::blocked_range<std::vector<double>::iterator >(vec.begin(), vec.end()),0.,
                               [](const tbb::blocked_range<std::vector<double>::iterator >& r, double init)->double {
                                   for(std::vector<double>::iterator a = r.begin(); a != r.end(); ++a)
                                     init += *a; 
                                   return init;
                                 },  
                               [](const double a, double b){ 
                                   return a+b;
                                 }   
                        );  
}
int main(){
  std::vector<double> largeVector(1000000);
  for (int i = 0; i < 1000000; ++i){ largeVector.at(i) = i;} 
  double a = ParallelSum(largeVector);
  double b = 0;
  b = std::accumulate(largeVector.begin(),largeVector.end(),0);
  std::cout << a << std::endl;
  std::cout << b << std::endl;
  return 0;
}
