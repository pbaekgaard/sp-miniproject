#include "reaction.hpp"
#include "vessel.hpp"
#include <gtest/gtest.h>

TEST(Vessel, Can_Add_reactant) //R9
{
    auto v = Vessel{""};
    const auto D_A = v.add("D_A", static_cast<size_t>(0));
    EXPECT_EQ(D_A, v.reactantTable.get("D_A"));
}

TEST(Vessel, Can_Add_Reaction)//R9
{
    auto v = Vessel{"herro"};
    const auto D_A = v.add("D_A", 0);
    const auto gammaA = 1;
    const auto A = v.add("A", 0);
    const auto DA = v.add("DA", 1);
    v.add((A + DA) >> gammaA >>= D_A);
}
