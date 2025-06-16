#include "observer.hpp"
#include "seihr.hpp"
#include <benchmark/benchmark.h>

static void bm_covid_19(benchmark::State &state)
{
    auto observer = PeakHospitalizedObserver{};
    auto v = seihr(state.range(0));
    const auto time_units = 100;
    const auto number_of_threads = state.range(1);
    const auto number_of_simulations = state.range(2);
    std::vector<SymbolTable<std::string, Reactant>> peaks;

    for (auto _ : state)
    {
        for (auto i = 0; i < number_of_simulations; i += number_of_threads)
        {
            auto result = v.runSimulations(number_of_threads, time_units, observer);
        }
    }
}

// R10
BENCHMARK(bm_covid_19)
    ->Args(std::vector<std::int64_t>{589755, 1, 100})
    ->Args(std::vector<std::int64_t>{589755, 100, 100});

BENCHMARK_MAIN();
