#include "vessel.hpp"
#include <QApplication>
#include <gtest/gtest.h>
#include <tuple>

TEST(SIMULATION, updates_reactants) //R9 + R7
{
    auto app = createApp();

    auto v = Vessel{"Jutlandia"};
    const auto A = v.add("A", 1);
    const auto B = v.add("B", 1);
    const auto C = v.add("C", 0);
    const auto gamma = 1;
    v.add((A + B) >> gamma >>= C);
    auto chartObserver = ChartObserver("update_reactions_test");
    auto [reactantTableResults, reactionTableResults] =
        v.runSimulations(1, 48, chartObserver); // shows chart window but doesn't block
    chartObserver.finalize(48);
    // Quit the app after 3 seconds
    // Start Qt event loop (this will show the chart window)
    app->exec();

    // After event loop quits, check results:
    EXPECT_EQ(reactantTableResults.get("A").quantity, 0);
    EXPECT_EQ(reactantTableResults.get("B").quantity, 0);
    EXPECT_EQ(reactantTableResults.get("C").quantity, 1);
}

TEST(SIMULATION, multithreading) //R9
{

    auto v = Vessel{"Jutlandia"};
    const auto A = v.add("A", 1);
    const auto B = v.add("B", 1);
    const auto C = v.add("C", 0);
    const auto gamma = 1;
    v.add((A + B) >> gamma >>= C);

    auto [reactantTableResults, reactionTableResults] =
        v.runSimulations(10, 1000); // shows chart window but doesn't block

    // Quit the app after 3 seconds
    // Start Qt event loop (this will show the chart window)
    // app->exec();

    // After event loop quits, check results:
    EXPECT_EQ(reactantTableResults.get("A").quantity, 0);
    EXPECT_EQ(reactantTableResults.get("B").quantity, 0);
    EXPECT_EQ(reactantTableResults.get("C").quantity, 1);
}

TEST(SIMULATION, requirement5_figure1_1) { //R5 + R7 + R9
    auto app = createApp();

    auto v = Vessel{"Figure 1.1"};
    const auto A = v.add("A", 100);
    const auto B = v.add("B", 0);
    const auto C = v.add("C", 1);
    const auto gamma = 0.001;
    v.add((A + C) >> gamma >>= (B+C));
    auto chartObserver = ChartObserver("Figure 1.1");
    auto [reactantTableResults, reactionTableResults] =
        v.runSimulations(1, 2000, chartObserver); // shows chart window but doesn't block
    chartObserver.finalize(2000);
    app->exec(); //R6
}

TEST(SIMULATION, requirement5_figure1_2) { //R5 + R7 + R9
    auto app = createApp();

    auto v = Vessel{"Figure 1.2"};
    const auto A = v.add("A", 100);
    const auto B = v.add("B", 0);
    const auto C = v.add("C", 2);
    const auto gamma = 0.001;
    v.add((A + C) >> gamma >>= (B+C));
    auto chartObserver = ChartObserver("Figure 1.2");
    auto [reactantTableResults, reactionTableResults] =
        v.runSimulations(1, 1500, chartObserver); // shows chart window but doesn't block
    chartObserver.finalize(1500);
    app->exec(); //R6
}

TEST(SIMULATION, requirement5_figure1_3) {//R5 + R7 + R9
    auto app = createApp();

    auto v = Vessel{"Figure 1.3"};
    const auto A = v.add("A", 50);
    const auto B = v.add("B", 50);
    const auto C = v.add("C", 1);
    const auto gamma = 0.001;
    v.add((A + C) >> gamma >>= (B+C));
    auto chartObserver = ChartObserver("Figure 1.3");
    auto [reactantTableResults, reactionTableResults] =
        v.runSimulations(1, 1500, chartObserver); // shows chart window but doesn't block
    chartObserver.finalize(1500);
    app->exec(); //R6
}
