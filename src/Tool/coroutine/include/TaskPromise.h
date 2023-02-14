#pragma once

#ifndef ZTOOL_TASKPROMISE_H
#define ZTOOL_TASKPROMISE_H

#include <coroutine>
#include <functional>
#include <list>
#include <mutex>
#include <optional>

#include "Result.h"
#include "TaskAwaiter.h"

namespace Ztool {

template <typename ResultType>
struct TaskPromise {
  // 协程进入后立刻执行
  std::suspend_never initial_suspend() { return {}; }

  // 协程退出前先挂起，避免结果被销毁
  std::suspend_always final_suspend() noexcept { return {}; }

  Task<ResultType> get_return_object() {
    return Task{std::coroutine_handle<TaskPromise>::from_promise(*this)};
  }

  template <typename _ResultType>
  TaskAwaiter<_ResultType> await_transform(Task<_ResultType>&& task) {
    return TaskAwaiter<_ResultType>(std::move(task));
  }

  void unhandled_exception() {
    std::lock_guard lock(mCompletionLock);
    mResult = Result<ResultType>(std::current_exception());
    mCompletion.notify_all();
    NotifyCallbacks();
  }

  void return_value(ResultType value) {
    std::lock_guard lock(mCompletionLock);
    mResult = Result<ResultType>(std::move(value));
    mCompletion.notify_all();
    NotifyCallbacks();
  }

  ResultType get_result() {
    std::unique_lock lock(mCompletionLock);
    if (!mResult.has_value()) {
      mCompletion.wait(lock);
    }
    return mResult->getOrThrow();
  }

  void on_completed(std::function<void(Result<ResultType>)>&& func) {
    std::unique_lock lock(mCompletionLock);
    if (mResult.has_value()) {
      auto value = mResult.value();
      lock.unlock();
      func(value);
    } else {
      mCompletionCallbacks.push_back(func);
    }
  }

 private:
  void NotifyCallbacks() {
    auto value = mResult.value();
    for (auto& callback : mCompletionCallbacks) {
      callback(value);
    }
    mCompletionCallbacks.clear();
  }

 private:
  std::optional<Result<ResultType>> mResult;

  std::mutex mCompletionLock;
  std::condition_variable mCompletion;

  std::list<std::function<void(Result<ResultType>)>> mCompletionCallbacks;
};
}  // namespace Ztool
#endif  // ZTOOL_TASKPROMISE_H