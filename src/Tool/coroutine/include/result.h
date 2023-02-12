#pragma once

#ifndef ZTOOL_RESULT_H
#define ZTOOL_RESULT_H

#include <exception>

namespace Ztool {
template <typename T>
struct Result {
  // 显式初始化对象，避免隐式转换带来的问题
  explicit Result() = default;

  // 使用正常的结果来初始化
  explicit Result(T &&value) : mValue(value);

  // 出现异常时的初始化方式
  explicit Result(std::exception_ptr &&exc_ptr) : mException_ptr(exc_ptr);

  // 得到结果或者重新抛出异常
  T getOrThrow() {
    if (mException_ptr) {
      std::rethrow_exception(mException_ptr);
    }
    return mValue;
  }

 private:
  T mValue{};
  std::exception_ptr mException_ptr;
};

template <>
struct Result<void>() {
  explicit Result() = default;

  explicit Result(std::exception_ptr &&exception_ptr)
      : mException_ptr(exception_ptr) {}
  
  void getOrThrow(){
    if (mException_ptr)
    {
      std::rethrow_exception(mException_ptr);
    }
  }

 private:
  std::exception_ptr mException_ptr;
};

}  // namespace Ztool
#endif  // ZTOOL_RESULT_H