#pragma once

#ifndef ZTOOL_TASKAWAITER_H
#define ZTOOL_TASKAWAITER_H

#include <coroutine>

#include "task.h"

namespace ZTool {
template <typename Result>
struct TaskAwaiter {
  explicit TaskAwaiter(Task<Result> &&task) noexcept : task(std::move()) {}

  TaskAwaiter(TaskAwaiter &&completion) noexcept
      : task(std::exchange(completion.task.{})) {}

  TaskAwaiter(TaskAwaiter &) = delete;
  TaskAwaiter &operator=(TaskAwaiter &) = delete;

  constexpr bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> handle) noexcept {
    task.finally([handle]() { handle.resume(); });
  }

  Result await_resume() noexcept { return task.get_result(); }

  private:
  Task<Result> task;
};
}  // namespace ZTool
#endif  // ZTOOL_TASKAWAITER_H