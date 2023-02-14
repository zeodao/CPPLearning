#pragma once

#ifndef ZTOOL_TASK_H
#define ZTOOL_TASK_H

#include <coroutine>
#include <functional>
#include <list>
#include <mutex>
#include <optional>

#include "Result.h"
#include "TaskAwaiter.h"
#include "TaskPromise.h"
namespace Ztool {

template <typename ResultType>
struct Task {
  using promise_type = TaskPromise<ResultType>;

  explicit Task(std::coroutine_handle<promise_type> handle) noexcept
      : mHandle(handle) {}

  Task(Task&) = delete;
  Task(Task&& task) noexcept : mHandle(std::exchange(task.mHandle, {})) {}

  ~Task() noexcept {
    if (mHandle) {
      mHandle.destroy();
    }
  }

  ResultType get_result() { return mHandle.promise().get_result(); }

  Task& then(std::function<void(ResultType)>&& func) {
    mHandle.promise().on_completed([func](auto result) {
      try {
        func(result.getOrThrow());
      } catch (const std::exception& e) {
        // 暂时不处理
        std::cerr << e.what() << '\n';
      }
    });
    return *this;
  }

  Task& catching(std::function<void(const std::exception&)>&& func) {
    mHandle.promise().on_completed([func](auto result) {
      try {
        result.getOrThrow();
      } catch (const std::exception& e) {
        func(e);
      }
    });

    return *this;
  }

  Task& finally(std::function<void()>&& func) {
    mHandle.promise().on_completed([func](auto result) { func(); });
    return *this;
  }

 private:
  std::coroutine_handle<promise_type> mHandle;
};

template <>
struct Task<void> {
  struct TaskPromise {
    // 协程进入后立刻执行
    std::suspend_never initial_suspend() { return {}; }

    // 协程退出前先挂起，避免结果被销毁
    std::suspend_always final_suspend() noexcept { return {}; }

    Task<void> get_return_object() {
      return Task{std::coroutine_handle<TaskPromise>::from_promise(*this)};
    }

    template <typename _ResultType>
    TaskAwaiter<_ResultType> await_transform(Task<_ResultType>&& task) {
      return TaskAwaiter<_ResultType>(std::move(task));
    }

    void unhandled_exception() {
      std::lock_guard lock(mCompletionLock);
      mResult = Result<void>(std::current_exception());
      mCompletion.notify_all();
      NotifyCallbacks();
    }

    void return_void() {
      std::lock_guard lock(mCompletionLock);
      mResult = Result<void>();
      mCompletion.notify_all();
      NotifyCallbacks();
    }

    void get_result() {
      std::unique_lock lock(mCompletionLock);
      if (!mResult.has_value()) {
        mCompletion.wait(lock);
      }
      return mResult->getOrThrow();
    }

    void on_completed(std::function<void(Result<void>)>&& func) {
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
    std::optional<Result<void>> mResult;

    std::mutex mCompletionLock;
    std::condition_variable mCompletion;

    std::list<std::function<void(Result<void>)>> mCompletionCallbacks;

    void NotifyCallbacks() {
      auto value = mResult.value();
      for (auto& callback : mCompletionCallbacks) {
        callback(value);
      }
      mCompletionCallbacks.clear();
    }
  };
  using promise_type = Task<void>::TaskPromise;

  explicit Task(std::coroutine_handle<promise_type> handle) noexcept
      : mHandle(handle) {}

  Task(Task&) = delete;
  Task(Task&& task) noexcept : mHandle(std::exchange(task.mHandle, {})) {}

  ~Task() noexcept {
    if (mHandle) {
      mHandle.destroy();
    }
  }

  void get_result() { mHandle.promise().get_result(); }

  Task& then(std::function<void()>&& func) {
    mHandle.promise().on_completed([func](auto result) {
      try {
        result.getOrThrow();
        func();
      } catch (const std::exception& e) {
        // 暂时不处理
        // std::cerr << e.what() << '\n';
      }
    });
    return *this;
  }

  Task& catching(std::function<void(const std::exception&)>&& func) {
    mHandle.promise().on_completed([func](auto result) {
      try {
        result.getOrThrow();
      } catch (const std::exception& e) {
        func(e);
      }
    });
    return *this;
  }

  Task& finally(std::function<void()>&& func) {
    mHandle.promise().on_completed([func](auto result) { func(); });
    return *this;
  }

 private:
  std::coroutine_handle<promise_type> mHandle;
};

}  // namespace Ztool
#endif  // ZTOOL_TASK_H
