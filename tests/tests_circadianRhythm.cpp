#include "reactant.hpp"
#include "reaction.hpp"
#include "vessel.hpp"
#include <QApplication>
#include <QChartView>
#include <QLineSeries>
#include <QTimer>
#include <QValueAxis>
#include <gtest/gtest.h>

Vessel circadian_rhythm()
{
    const auto alphaA = 50;
    const auto alpha_A = 500;
    const auto alphaR = 0.01;
    const auto alpha_R = 50;
    const auto betaA = 50;
    const auto betaR = 5;
    const auto gammaA = 1;
    const auto gammaR = 1;
    const auto gammaC = 2;
    const auto deltaA = 1;
    const auto deltaR = 0.2;
    const auto deltaMA = 10;
    const auto deltaMR = 0.5;
    const auto thetaA = 50;
    const auto thetaR = 100;

    auto v = Vessel("Circadian");

    const auto env = v.environment();
    const auto DA = v.add("DA", 1);
    const auto D_A = v.add("D_A", 0);
    const auto DR = v.add("DR", 1);

    const auto D_R = v.add("D_R", 0);
    const auto MA = v.add("MA", 0);
    const auto MR = v.add("MR", 0);
    const auto A = v.add("A", 0);
    const auto R = v.add("R", 0);
    const auto C = v.add("C", 0);

    v.add((A + DA) >> gammaA >>= D_A);
    v.add(D_A >> thetaA >>= DA + A);
    v.add((A + DR) >> gammaR >>= D_R);
    v.add(D_R >> thetaR >>= DR + A);
    v.add(D_A >> alpha_A >>= MA + D_A);
    v.add(DA >> alphaA >>= MA + DA);
    v.add(D_R >> alpha_R >>= MR + D_R);
    v.add(DR >> alphaR >>= MR + DR);
    v.add(MA >> betaA >>= MA + A);
    v.add(MR >> betaR >>= MR + R);
    v.add((A + R) >> gammaC >>= C);
    v.add(C >> deltaA >>= R);
    v.add(A >> deltaA >>= env);
    v.add(R >> deltaR >>= env);
    v.add(MA >> deltaMA >>= env);
    v.add(MR >> deltaMR >>= env);
    return v;
}

TEST(CircadianRhythm, full_example)
{
    int argc = 1;
    char *argv[] = {(char *)"test", nullptr};
    QApplication app(argc, argv);
    const auto alphaA = 50;
    const auto alpha_A = 500;
    const auto alphaR = 0.01;
    const auto alpha_R = 50;
    const auto betaA = 50;
    const auto betaR = 5;
    const auto gammaA = 1;
    const auto gammaR = 1;
    const auto gammaC = 2;
    const auto deltaA = 1;
    const auto deltaR = 0.2;
    const auto deltaMA = 10;
    const auto deltaMR = 0.5;
    const auto thetaA = 50;
    const auto thetaR = 100;

    auto v2 = Vessel("Circadian");

    const auto env = v2.environment();
    const auto DA = v2.add("DA", 1);
    const auto D_A = v2.add("D_A", 0);
    const auto DR = v2.add("DR", 1);

    const auto D_R = v2.add("D_R", 0);
    const auto MA = v2.add("MA", 0);
    const auto MR = v2.add("MR", 0);
    const auto A = v2.add("A", 0);
    const auto R = v2.add("R", 0);
    const auto C = v2.add("C", 0);
    auto v = circadian_rhythm();
    // std::cout << "REACTANT TABLE:" << std::endl;
    // v.reactantTable.print();
    // std::cout << "\n\n\n\nREACTION TABLE:" << std::endl;
    // v.reactionTable.print();
    const auto reaction = v.reactionTable.get(((A + DA) >> gammaA >>= D_A).hash());
    EXPECT_TRUE((std::find(reaction.inputs.begin(), reaction.inputs.end(), A) != reaction.inputs.end()));
    EXPECT_TRUE((std::find(reaction.inputs.begin(), reaction.inputs.end(), DA) != reaction.inputs.end()));
    EXPECT_TRUE(reaction.rate == gammaA);
    EXPECT_TRUE((std::find(reaction.products.begin(), reaction.products.end(), D_A) != reaction.products.end()));
    EXPECT_FALSE((std::find(reaction.products.begin(), reaction.products.end(), MA) != reaction.products.end()));

    const auto reactant = v.reactantTable.get("DA");
    const auto reactant2 = v.reactantTable.get("D_A");
    EXPECT_TRUE(reactant.quantity == 1);
    EXPECT_TRUE(reactant.name == "DA");
    EXPECT_TRUE(reactant2.quantity == 0);
    EXPECT_TRUE(reactant2.name == "D_A");
    v.generateGraph();
    v.simulate(48, 1); // shows chart window but doesn't block

    // Start Qt event loop (this will show the chart window)
    app.exec();
}
