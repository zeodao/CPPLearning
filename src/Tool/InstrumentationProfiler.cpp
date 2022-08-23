#include "./include/InstrumentationProfiler.h"
namespace Ztool {
static constexpr size_t n = 1 << 28;
std::vector<float> a(n);

static float CalculateFunc1(float x) {
  return sinf(x) + cosf((x + 1) * x) * sqrtf(x * (x + 1));
}

static float CalculateFunc2(float x) { return sqrtf(x) * x; }

#pragma optimize("", off)
void TestFunciton1() {
  ZTOOLPROFILE_FUNCTION();
  for (size_t i = 0; i < n; i++) {
    a[i] = CalculateFunc1(i);
  }
}
#pragma optimize("", on)

#pragma optimize("", off)
void TestFunciton2() {
  ZTOOLPROFILE_FUNCTION();
  for (size_t i = 0; i < n; i++) {
    a[i] = CalculateFunc2(i);
  }
}
#pragma optimize("", on)

void BenchMarkTest() {
  ZTOOLPROFILE_FUNCTION();
  TestFunciton1();
  TestFunciton2();
}
}  // namespace Ztool
