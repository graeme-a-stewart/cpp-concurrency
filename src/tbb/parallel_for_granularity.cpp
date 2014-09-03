/*
illustration of granularity using TBB parallel_for
*/

#include <iostream>
#include <tbb/tbb.h>
#include <tbb/parallel_for.h>
 
long len;	
float *set1;
float *set2;
float *set3;
 
class GrainTest {

    public:

        void operator()(const tbb::blocked_range<size_t>& r) const{ 
            std::cout << r.begin() << std::endl;
            for (long i = r.begin(); i != r.end(); ++i){
                set3[i] = (set1[i] * set2[i]) / 2.0 + set2[i];
            }
        }

};

int main() {

    len = 5000;
    set1 = new float[len];
    set2 = new float[len];
    set3 = new float[len];
    tbb::parallel_for(tbb::blocked_range<size_t>(0, len, 100), GrainTest());
    return 0;

}
