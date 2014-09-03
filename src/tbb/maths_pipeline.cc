#include <iostream>
#include <cstdio>
#include <cmath>
#include <tbb/tbb.h>

class DataReader {
private:
  FILE *my_input;
 
public:
  DataReader(FILE* in):
    my_input{in} {};

  DataReader(const DataReader& a):
    my_input{a.my_input} {};

  ~DataReader() {};

  double operator()(tbb::flow_control& fc) const {
    double number=-1.0;
    int rc = fscanf(my_input, "%f", &number);
    //std::cout << rc << " " << number << std::endl;
    if (rc != 1) {
      fc.stop();
      return 0.0;
    }
    return number;
  }
};


class Transform {
public:
  double operator()(double const number) const {
    std::cout << "Running transform " << number << std::endl;
    double answer;
    if (number <= 0.0)
      return 0.0;
    else
      return log(answer);
  }
};

class DataWriter {
private:
  FILE* my_output;

public:
  DataWriter(FILE* out):
    my_output{out} {};

  void operator()(double const answer) const {
    fprintf(my_output, "%f\n", answer);
  }
};


void runFilter(int ntoken, FILE* input_file, FILE* output_file) {
  tbb::parallel_pipeline(ntoken,
			 tbb::make_filter<void, double>(tbb::filter::serial_in_order, DataReader(input_file))
			 &
			 tbb::make_filter<double, double>(tbb::filter::parallel, Transform())
			 &
			 tbb::make_filter<double, void>(tbb::filter::serial_in_order, DataWriter(input_file))
			 );
}



int main() {
  const char in_file[] = "input.txt";
  const char out_file[] = "output.txt";

  const int ntoken = 100;

  FILE* in = fopen(in_file, "r");
  FILE* out = fopen(out_file, "w");

  runFilter(ntoken, in, out);

  std::cout << "Success" << std::endl;

  fclose(in);
  fclose(out);
}
