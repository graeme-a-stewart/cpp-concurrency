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
    int rc = fscanf(my_input, "%lf\n", &number);
#ifdef DEBUG
    std::cout << "input " << rc << " " << number << std::endl;
#endif
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
#ifdef DEBUG
    std::cout << "Running transform " << number << std::endl;
#endif
    double answer=0.0;
    if (number > 0.0)
      for (int i=0; i<1000; ++i)
	answer += pow(log(number+answer)+1.0, 2.5);
    return answer;
  }
};

class DataWriter {
private:
  FILE* my_output;

public:
  DataWriter(FILE* out):
    my_output{out} {};

  DataWriter(const DataWriter& a):
    my_output{a.my_output} {};

  ~DataWriter() {};

  void operator()(double const answer) const {
#ifdef DEBUG
    std::cout << "Output " << answer << "(" << my_output << ")" << std::endl;
#endif
    fprintf(my_output, "%lf\n", answer);
  }
};


void runFilter(int ntoken, FILE* input_file, FILE* output_file) {
  tbb::parallel_pipeline(ntoken,
    tbb::make_filter<void, double>(tbb::filter::serial_in_order, DataReader(input_file))
    &
    tbb::make_filter<double, double>(tbb::filter::parallel, Transform())
    &
    tbb::make_filter<double, void>(tbb::filter::serial_in_order, DataWriter(output_file))
    );
}



int main(int argc, char* argv[]) {
  const char in_file[] = "input.txt";
  const char out_file[] = "output.txt";

  int ntoken = 10;
  if (argc == 2)
    ntoken = atoi(argv[1]);

  std::cout << "Running pipeline with " << ntoken << " tokens" << std::endl;

  FILE* in = fopen(in_file, "r");
  if (!in) {
    std::cerr << "Failed to open input file " << in_file << std::endl;
    return 1;
  }
  FILE* out = fopen(out_file, "w");
  if (!out) {
    std::cerr << "Failed to open input file " << in_file << std::endl;
    return 1;
  }

  auto t0 = tbb::tick_count::now();

  runFilter(ntoken, in, out);

  auto t1 = tbb::tick_count::now();
  auto parallel_tick_interval = t1-t0;
  std::cout << "Pipeline took " << parallel_tick_interval.seconds() << "s" << std::endl;

  std::cout << "Success" << std::endl;

  fclose(in);
  fclose(out);
}
