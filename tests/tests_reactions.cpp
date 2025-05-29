#include "reactant.hpp"
#include "reaction.hpp"
#include <gtest/gtest.h>

TEST(Reaction, chain_reactions)
{
    const auto r = Reactant("A", 0);
    const auto r2 = Reactant("DA", 1);
    const auto r3 = Reactant("D_A",0);
    Reaction reaction = r + r2 + r3;
    const auto expected_reactant_list = std::vector<Reactant>{r, r2, r3};
    EXPECT_EQ(reaction.inputs, expected_reactant_list);
};

TEST(Reaction, can_add_rate) {
    const auto r = Reactant("A", 0);
    const auto r2 = Reactant("DA", 1);
    const auto r3 = Reactant("D_A",0);
    Reaction reaction = r + r2 + r3;

    EXPECT_EQ(reaction.rate, 0);
    reaction >> 1;
    EXPECT_EQ(reaction.rate, 1);
}

TEST(Reaction, create_full_reaction) {
    const auto A = Reactant("A", 0);
    const auto DA = Reactant("DA", 1);
    const auto D_A = Reactant("D_A",0);
    const auto gamma_A = 1;
    Reaction reaction = (A+DA) >> gamma_A >>= D_A;
    std::vector<Reactant> inputs {A, DA};
    std::vector<Reaction> products {D_A};
    EXPECT_EQ(reaction.inputs, inputs);
    EXPECT_EQ(reaction.products, products);
    EXPECT_EQ(reaction.rate, gamma_A);
}