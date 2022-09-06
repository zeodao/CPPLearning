#include <iostream>

#include "./Tool/include/InstrumentationProfiler.h"
#include "./cppFeature/coroutines/include/coroutineTest.h"

int main(int argc, char *argv[]) {
  auto gen = Ztool::fibonacci_sequence(10);
  std::cout << "finish coroutine Create!" << std::endl;
  for (int i = 0; gen; i++) {
    std::cout << "fib(" << i << ")=" << gen() << std::endl;
  }

  std::cout << "Hello,world" << std::endl;
  return 0;
}
