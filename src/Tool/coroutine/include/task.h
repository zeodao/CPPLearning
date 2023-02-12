#include <coroutine>
#include <functional>

#include "result.h"
#include "taskPromise.h"
namespace Ztool {

template <typename ResultType>
struct Task {
  // 如果不在内部定义promisetype 使用别名引入
  using promise_type = TaskPromise<ResultType>;

  explicit Task(std::coroutine_handle<promise_type> handle) noexcept
      : mHandle(handle) {}

  Task(Task&) = delete;
  Task(Task&& task) noexcept : mHandle(std::exchange(task.mHandle, {})) {}

  ~Task() noexcept{
    if (mHandle) {
      mHandle.destroy();
    }
  }

  ResultType get_result() { return mHandle.promise().get_result(); }

  Task& then(std::function<void(ResultType)>&& func) {
    mHandle.promise().on_complete([func](auto result) {
      try {
        func(result.getOrThrow());
      } catch (const std::exception& e) {
        // 暂时不处理
        std::cerr << e.what() << '\n';
      }
    });
    return *this;
  }

  Task& catch (std::function<void(std::exception&)>&& func) {
    mHandle.promise().on_complete([func](auto result) {
      try {
        result.getOrThrow();
      } catch (const std::exception& e) {
        func(e);
      }
      return *this;
    });
  }

  Task& finally(std::function<void()>&& func) {
    mHandle.promise().on_completed([func](auto result) {
      try {
        result.getOrThrow();
      } catch (const std::exception& e) {
        func(e);
      }
      return *this;
    })
  }

 private:
  std::coroutine_handle<promise_type> mHandle;
};



}  // namespace Ztool
