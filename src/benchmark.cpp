#include <benchmark/benchmark.h>

#include <array>

size_t IntergerAlign(size_t num, size_t align) {
  return (num + align - 1) / align * align;
}

size_t IntergerAlignCeil(size_t num, size_t align) {
  return ceil(static_cast<float>(num) / align) * align;
}
template <typename T>
T RandT(T min, T max) {
  T temp;
  if (min > max) {
    temp = max;
    max = min;
    min = temp;
  }
  return rand() / (double)RAND_MAX * (max - min) + min;
}

static void BM_Align(benchmark::State& state) {
  std::array<size_t, 8192> testArray, ans;
  std::for_each(testArray.begin(), testArray.end(),
                [](size_t& n) { n = RandT<size_t>(4, 8192); });

  for (auto _ : state) {
    for (size_t i = 0; i < 8192; i++){
        ans[i] = IntergerAlign(testArray[i], 8);
    }
  }
  benchmark::DoNotOptimize(ans);
}

static void BM_Align_Ceil(benchmark::State& state) {
  std::array<size_t, 8192> testArray, ans;
  std::for_each(testArray.begin(), testArray.end(),
                [](size_t& n) { n = RandT<size_t>(4, 8192); });

  for (auto _ : state) {
    for (size_t i = 0; i < 8192; i++){
        ans[i] = IntergerAlignCeil(testArray[i], 8);
    }
  }
  benchmark::DoNotOptimize(ans);
}
BENCHMARK(BM_Align);
BENCHMARK(BM_Align_Ceil);

BENCHMARK_MAIN();