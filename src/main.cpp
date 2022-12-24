#include <iostream>
#include <numeric>

#include "./Tool/include/InstrumentationProfiler.h"
#include "./cppFeature/coroutines/include/coroutineTest.h"

struct plot
{
  std::vector<int> vint;
  int int1;
  float flo1;
  std::string he;
};


int main(int argc, char *argv[]) {
  plot plotS{{1,2,3,4}, 1, 1.0f, std::string("test Struct")};
  std::cout << plotS.int1 << std::endl;
}