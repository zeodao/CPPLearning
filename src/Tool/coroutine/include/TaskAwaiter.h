#pragma once

#ifndef ZTOOL_TASKAWAITER_H
#define ZTOOL_TASKAWAITER_H

#define __cpp_lib_corutine
#include <coroutine>

namespace Ztool {
template<typename ResultType>
struct Task;

template <typename Result>
struct TaskAwaiter {
  explicit TaskAwaiter(Task<Result> &&task) noexcept : mTask(std::move(task)) {}

  TaskAwaiter(TaskAwaiter &&completion) noexcept
      : mTask(std::exchange(completion.mTask, {})) {}

  TaskAwaiter(TaskAwaiter &) = delete;
  TaskAwaiter &operator=(TaskAwaiter &) = delete;

  constexpr bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> handle) noexcept {
    mTask.finally([handle]() { handle.resume(); });
  }

  Result await_resume() noexcept { return mTask.get_result(); }

 private:
  Task<Result> mTask;
};

}  // namespace Ztool
#endif  // ZTOOL_TASKAWAITER_H