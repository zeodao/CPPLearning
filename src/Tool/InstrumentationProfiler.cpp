#include "./include/InstrumentationProfiler.h"
namespace Ztool {
static constexpr size_t n = 1 << 28;
std::vector<float> a(n);
static float CalculateFunc(float x) {
  return sinf(x) + cosf((x + 1) * x) * sqrtf(x * (x + 1));
}
#pragma optimize("", off)
void TestFunciton1() {
  ZTOOLPROFILE_FUNCTION();
  for (size_t i = 0; i < n; i++) {
    a[i] = CalculateFunc(i);
  }
}
#pragma optimize("", on)
void BenchMarkTest() {}
} // namespace Ztool
