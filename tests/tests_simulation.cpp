#include "vessel.hpp"
#include <QApplication>
#include <gtest/gtest.h>
#include <tuple>

TEST(SIMULATION, updates_reactants)
{
    int argc = 1;
    char *argv[] = {(char *)"test", nullptr};
    QApplication app(argc, argv);

    auto v = Vessel{"Jutlandia"};
    const auto A = v.add("A", 1);
    const auto B = v.add("B", 1);
    const auto C = v.add("C", 0);
    const auto gamma = 1;
    v.add((A + B) >> gamma >>= C);

    auto [reactantTableResults, reactionTableResults] =
        v.runSimulations(1, 1000); // shows chart window but doesn't block

    // Quit the app after 3 seconds
    // Start Qt event loop (this will show the chart window)
    // app.exec();

    // After event loop quits, check results:
    EXPECT_EQ(reactantTableResults.get("A").quantity, 0);
    EXPECT_EQ(reactantTableResults.get("B").quantity, 0);
    EXPECT_EQ(reactantTableResults.get("C").quantity, 1);
}

TEST(SIMULATION, multithreading)
{
    int argc = 1;
    char *argv[] = {(char *)"test", nullptr};
    QApplication app(argc, argv);

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
    // app.exec();

    // After event loop quits, check results:
    EXPECT_EQ(reactantTableResults.get("A").quantity, 0);
    EXPECT_EQ(reactantTableResults.get("B").quantity, 0);
    EXPECT_EQ(reactantTableResults.get("C").quantity, 1);
}
