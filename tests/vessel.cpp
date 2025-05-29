#include "vessel.hpp"
#include "reaction.hpp"
#include <gtest/gtest.h>

TEST(Vessel, Can_Add_reactant)
{
    auto v = Vessel{"herro"};
    const auto D_A = v.add("D_A", static_cast<size_t>(0));
}

TEST(Reaction, chain_reactions)
{
    const auto r = Reactant();
    const auto r2 = Reactant();
    const auto r3 = Reactant();
    const Reaction reaction = r + r2 + r3;
}

// TEST(Vessel, Can_Add_Reaction)
// {
//     auto v = Vessel{"herro"};
//     const auto D_A = v.add("D_A", 0);
//     const auto gammaA = 1;
//     const auto A = v.add("A", 0);
//     const auto DA = v.add("DA", 1);
//     v.add((A + DA) >> gammaA >>= D_A);
// }
