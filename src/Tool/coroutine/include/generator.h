
#pragma once
#include <coroutine>
#include <exception>
#include <iostream>
namespace Ztool {
template <typename T>
struct Generator {
  struct promise_type {
    T value_;
    std::exception_ptr exception_;
    Generator get_return_object() {
      return Generator(
          std::coroutine_handle<promise_type>::from_promise(*this));
    }
    std::suspend_always initial_suspend() noexcept { return {}; }

    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() { exception_ = std::current_exception(); }
    template <std::convertible_to<T> From>  // c++20 concept
    std::suspend_always yield_value(From&& from) {
      value_ = std::forward<From>(from);  // caching the result in promise
      return {};
    }
    void return_void() {}
  };

  std::coroutine_handle<promise_type> h_;

  Generator(std::coroutine_handle<promise_type> h) : h_(h) {}
  ~Generator() { h_.destroy(); }
  explicit operator bool() {
    fill();
    return !h_.done();
  }

  T operator()() {
    fill();
    m_full = false;
    return std::move(h_.promise().value_);
  }

 private:
  bool m_full = false;

  void fill() {
    if (!m_full) {
      h_();
      if (h_.promise().exception_) {
        std::rethrow_exception(h_.promise().exception_);
      }
      m_full = true;
    }
  }
};
}  // namespace Ztool
