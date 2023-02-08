#include <iostream>
#include <numeric>

#include "./cppfeature/coroutines/include/coroutinetest.h"
#include "./tool/include/instrumentationprofiler.h"

namespace fooSpace {
template <typename T, T V>
constexpr char const* GetEnumName() {
  const char* info = __FUNCSIG__;
  char letter[4]{",:>"};
  size_t order = 0;
  size_t strPos[2]{0, 0};
  for (size_t i = 0; info[i] != '\0'; i++) {
    bool change{false};
    if (info[i] == letter[order]) {
      order++;
      change = true;
    }
    if (!change) {
      continue;
    }
    if (order == 2) {
      strPos[0] = i + 1;
    } else if (order == 3) {
      strPos[1] = i - 1;
      break;
    }
  }
  return info + strPos[0];
}
enum class num { one, two, three };
}  // namespace fooSpace

int main(int argc, char* argv[]) {
  std::cout << fooSpace::GetEnumName<fooSpace::num, fooSpace::num::one>()
            << std::endl;
}