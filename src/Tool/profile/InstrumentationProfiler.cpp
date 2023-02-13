#include "./include/InstrumentationProfiler.h"

#include <array>
namespace Ztool {
  void FuncA(){
    ZTOOLPROFILE_FUNCTION();
    std::chrono::milliseconds dura(200);
    std::this_thread::sleep_for(dura);
  }

  void FuncB(){
    ZTOOLPROFILE_FUNCTION();
    std::chrono::milliseconds dura(150);
    std::this_thread::sleep_for(dura);
  }

  void Func1(){
    ZTOOLPROFILE_FUNCTION();
    std::chrono::milliseconds dura(2000);
    std::this_thread::sleep_for(dura);
    FuncA();
  }

  void Func2(){
    ZTOOLPROFILE_FUNCTION();
    std::chrono::milliseconds dura(1500);
    std::this_thread::sleep_for(dura);
    FuncB();
  }

  void Func3(){
    ZTOOLPROFILE_FUNCTION();
    std::chrono::milliseconds dura(2500);
    std::this_thread::sleep_for(dura);
    FuncA();
    FuncB();
  }

  void SessionDemo(){
    Instrumentor::Get().BeginSession("SessionDemo");

    std::vector<std::thread> workers;

    workers.push_back(std::thread(Func1));
    workers.push_back(std::thread(Func2));
    workers.push_back(std::thread(Func3));

    for (auto& w : workers)
    {
      w.join();
    }

    Instrumentor::Get().EndSession();
  }
}  // namespace Ztool
