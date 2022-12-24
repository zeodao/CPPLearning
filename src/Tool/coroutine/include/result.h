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
  T GetOrThrow() {
    if (mException_ptr) {
      std::rethrow_exception(mException_ptr);
    }
    return mValue;
  }

 private:
  T mValue{};
  std::exception_ptr mException_ptr;
};

}  // namespace Ztool
