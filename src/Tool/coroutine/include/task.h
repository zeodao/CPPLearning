#include <coroutine>

namespace Ztool
{
  template<typename ResultType>
  struct TaskPromise
  {
    //协程进入
    std::suspend_never initial_suspend() {return {};}

    //协程推出

  };
   
} // namespace Ztool
