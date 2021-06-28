#include <benchmark/benchmark.h>

BENCHMARK_MAIN();

static void tmp(benchmark::State &state) {
    // init
    for (auto _ : state) {
        // time_test
    }
}

BENCHMARK(tmp)->Unit(benchmark::kMillisecond);