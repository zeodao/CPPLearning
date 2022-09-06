#include "./include/coroutineTest.h"
namespace Ztool {
Generator<uint64_t> fibonacci_sequence(unsigned n) {
  if (n == 0) co_return;
  if (n > 94) {
    throw std::runtime_error(
        "Too big Fibonacci sequence. Elements would oveflow");
  }
  co_yield 0;
  if (n == 1) co_return;

  co_yield 1;
  if (n == 2) co_return;

  uint64_t a = 0;
  uint64_t b = 1;
  for (unsigned i = 2; i < n; i++) {
    uint64_t s = a + b;
    co_yield s;
    a = b;
    b = s;
  }
}
}  // namespace Ztool
