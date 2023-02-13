#include <iostream>
#include <numeric>

#include "./Tool/coroutine/include/Task.h"
#include "./tool/profile/include/instrumentationprofiler.h"

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

Task<int> simple_task2() {
  Ztool::ZTOOLPROFILE_FUNCTION();
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(1s);
  co_return 2;
}

Task<int> simple_task3() {
  Ztool::ZTOOLPROFILE_FUNCTION();
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(2s);
  co_return 3;
}

Task<int> simple_task() {
  Ztool::ZTOOLPROFILE_FUNCTION();
  using namespace std::chrono_literals;
  auto result2 = co_await simple_task2();
  auto result3 = co_await simple_task3();
  co_return 1 + result2 + result3;
}

int main(int argc, char* argv[]) {
  auto simpleTask = simple_task();
  simpleTask.then([](int i) {}).catching([](std::exception& e) {});
  try {
    auto i = simpleTask.get_result();
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}