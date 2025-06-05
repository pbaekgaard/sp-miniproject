#pragma once
#include "reactant.hpp"
#include "reaction.hpp"
#include <functional>
namespace std
{
template <> struct hash<Reactant>
{
    size_t operator()(const Reactant &r) const noexcept
    {
        size_t h1 = std::hash<std::string>{}(r.name);
        size_t h2 = std::hash<size_t>{}(r.quantity);
        return h1 ^ (h2 << 1);
    }
};

template <> struct hash<Reaction>
{
    size_t operator()(const Reaction &r) const noexcept
    {
        size_t seed = 0;
        auto hash_combine = [&seed](size_t h) { seed ^= h + 0x9e3779b9 + (seed << 6) + (seed >> 2); };

        for (const auto &input : r.inputs)
            hash_combine(std::hash<Reactant>{}(input));

        for (const auto &product : r.products)
            hash_combine(std::hash<Reaction>{}(product));

        hash_combine(std::hash<size_t>{}(r.rate));
        hash_combine(std::hash<size_t>{}(r.delay));
        hash_combine(std::hash<int>{}(r.incrementAmount));

        return seed;
    }
};
} // namespace std
