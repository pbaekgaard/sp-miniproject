#include "reactant.hpp"
#include "reaction.hpp"
#include <gtest/gtest.h>

TEST(Reaction, chain_reactions) //R9
{
    const auto r = Reactant("A", 0);
    const auto r2 = Reactant("DA", 1);
    const auto r3 = Reactant("D_A", 0);
    Reaction reaction = r + r2 + r3;
    const auto expected_reactant_list = std::vector<Reactant>{r, r2, r3};
    EXPECT_EQ(reaction.inputs, expected_reactant_list);
};

TEST(Reaction, can_add_rate) //R9
{
    const auto r = Reactant("A", 0);
    const auto r2 = Reactant("DA", 1);
    const auto r3 = Reactant("D_A", 0);
    Reaction reaction = r + r2 + r3;

    EXPECT_EQ(reaction.rate, 0);
    reaction >> 1;
    EXPECT_EQ(reaction.rate, 1);
}

