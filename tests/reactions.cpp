#include "reactant.hpp"
#include "reaction.hpp"
#include <gtest/gtest.h>

TEST(Reaction, chain_reactions)
{
    const auto r = Reactant();
    const auto r2 = Reactant();
    const auto r3 = Reactant();
    Reaction reaction = r + r2 + r3;
    const auto expected_reactant_list = std::vector<Reactant>{r, r2, r3};
    EXPECT_EQ(reaction.inputs, expected_reactant_list);
};
