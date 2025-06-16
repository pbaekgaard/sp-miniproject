#include "observer.hpp"
#include "vessel.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include "seihr.hpp"

TEST(SEIHR, basetest)//R9
{
    const auto N = 1000;
    auto v = seihr(N);
    const auto reactant = v.reactantTable.get("E");
    EXPECT_EQ(reactant.name, "E");
    const auto eps = 0.0009;                          // initial fraction of infectious
    const auto E0 = size_t(std::round(eps * N * 15)); // initial exposed
    EXPECT_EQ(reactant.quantity, E0);

    const auto I0 = size_t(std::round(eps * N));                          // initial infectious
    const auto gamma = 1.0 / 3.1;                                         // recovery rate (I -> R) ~3.1 days
    const auto R = v.add("R", 0);                                         // removed/immune (recovered + dead)
    const auto I = v.add("I", I0);                                        // infectious
    const auto reaction = v.reactionTable.get((I >> gamma >>= R).hash()); // infectious becomes removed
    EXPECT_EQ(reaction.rate, gamma);
    EXPECT_EQ(reaction.inputs, std::vector<Reactant>{I});
    EXPECT_EQ(reaction.products, std::vector<Reaction>{R});
}

TEST(SEIHR, PeakHospitalized) //R7
{
    auto app = createApp();
    const auto N = 10000;
    auto v = seihr(N);
    auto peakHObserver = PeakHospitalizedObserver();
    auto chartObserver = ChartObserver("SEIHR");
    v.runSimulations(1, 100, peakHObserver, chartObserver);
    std::cout << peakHObserver.peakHospitalized;
    chartObserver.finalize(100);
    app->exec();
}
