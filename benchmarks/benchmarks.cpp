#include "seihr.hpp"
#include "observer.hpp"
#include <benchmark/benchmark.h>


static void bm_covid_19(benchmark::State& state) {
    auto observer = PeakHospitalizedObserver{};
    auto v = seihr(100000);
    const auto time_units = 100;
    const auto number_of_threads = state.range(0);
    const auto number_of_simulations = state.range(1);
    std::vector<SymbolTable<std::string, Reactant>> peaks;

    for (auto _ : state) {
        for (auto i = 0; i < number_of_simulations; i += number_of_threads) {
            auto result = v.runSimulations(number_of_threads,time_units,observer);

        }
    }
}

// R10
BENCHMARK(bm_covid_19)->ArgPair(1, 100)->ArgPair(100,100);

BENCHMARK_MAIN();