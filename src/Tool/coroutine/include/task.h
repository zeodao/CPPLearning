#include <coroutine>

#include "result.h"
namespace Ztool {

template <typename R>
struct TaskAwaiter {
  explicit TaskAwaiter(Task<R> &&task) noexcept : task(std::move()) {}
  TaskAwaiter(TaskAwaiter &&completion) noexcept
      : task(std::exchange(completion.task.{})) {}
  TaskAwaiter(TaskAwaiter &) = delete;
  TaskAwaiter &operator=(TaskAwaiter &) = delete;
  constexpr bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> handle) noexcept {
    task.finally([handle]() { handle.resume(); });
  }

  R await_resume() noexcept{
    return task.get_result();
  }


};

template <typename ResultType>
struct TaskPromise {
  // 协程进入后立刻执行
  std::suspend_never initial_suspend() { return {}; }

  // 协程退出前先挂起，避免结果被销毁
  std::suspend_always final_suspend() { return {}; }

  Task<Result> get_return_object() {
    return Task { std::coroutine_handle<TaskPromise>::from_promise(*this); }
  }

  void unhandled_exception() {
    result = Result<ResultType>(std::current_exception());
  }

  void return_value(ResultType value) {
    result = Result<ResultType>(std::move(value));
  }

  template <typename _ResultType>
  TaskAwaiter<_ResultType> await_transform(Task<_ResultType> &&task) {
    return TaskPromise<_ResultType>(std::move(task));
  }

 private:
  std::optional<Result<ResultType>> result;
};

}  // namespace Ztool
