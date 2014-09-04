#include <iostream>
#include <fstream>
#include <string>

const long INPUT_SIZE = 1000000;

void gen_input(char* fname) {
  long num = INPUT_SIZE;

  std::ofstream out;
  out.open(fname, std::ofstream::out);

  int a=0;
  int b=1;
  for( long j=0; j<num; ++j ) {
    out << double(a) << std::endl;
    b+=a;
    a=(b-a)%10000;
    if (a<0) a=-a;
  }

  out.close();
}


int main() {
  char fname[] = "input.txt";
  gen_input(fname);

  return 0;
}
