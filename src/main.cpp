#include <iostream>
#include "./Tool/include/InstrumentationProfiler.h"

int main (int argc, char *argv[])
{
  Ztool::Instrumentor::Get().BeginSession("Profile");
  Ztool::BenchMarkTest();
  Ztool::Instrumentor::Get().EndSession();
  std::cout << "Hello,world" << std::endl;
  return 0;
}
