#include <iostream>
#include <numeric>

#include "./cppfeature/coroutines/include/coroutinetest.h"
#include "./tool/include/instrumentationprofiler.h"

struct plot {
  std::vector<int> vint;
  int int1;
  float flo1;
  std::string he;
};

int main(int argc, char *argv[]) {
  plot plots{{1, 2, 3, 4}, 1, 1.0f, std::string("test struct")};
  std::cout << plots.int1 << std::endl;
}