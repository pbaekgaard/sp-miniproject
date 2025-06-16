#pragma once
#include <observer.hpp>

#include "reactant.hpp"
#include "reaction.hpp"
#include "symboltable.hpp"
#include <string>
#include <tuple>
template <typename T>
concept isObserver = std::is_base_of_v<Observer, std::remove_cvref_t<T>>;
class Vessel
{
  public:
    Vessel(std::string name);
    Vessel(Vessel &&) = default;
    Vessel(const Vessel &) = default;
    Vessel &operator=(Vessel &&) = default;
    Vessel &operator=(const Vessel &) = default;
    Reactant add(std::string item, std::size_t quantity); // Adds to the symbol table
    void add(Reaction reaction_specification);            // Adds to the symbol table
    SymbolTable<std::string, Reactant> reactantTable;
    SymbolTable<std::string, Reaction> reactionTable;
    Reactant environment()
    {
        return Reactant("Environment", 0);
    };
    void generateGraph() const;
    template <isObserver... Observers>

    auto runSimulations(std::size_t numberOfThreads, std::size_t endTime, Observers &&...observers)
        -> std::tuple<SymbolTable<std::string, Reactant>, SymbolTable<std::string, Reaction>>
    {
        if (numberOfThreads == 1)
        {
            return simulate(endTime, std::forward<Observers>(observers)...);
        }
        std::vector<std::future<std::tuple<SymbolTable<std::string, Reactant>, SymbolTable<std::string, Reaction>>>>
            futures;

        for (std::size_t i = 0; i < numberOfThreads; ++i)
        {
            if (i == 0)
            {

                futures.push_back(std::async(std::launch::async, [&, this]() mutable {
                    // Run one simulation; arguments can be customized
                    return this->simulate(
                        endTime,
                        std::forward<Observers>(observers)...); // e.g., endTime = 1000, sampleRate = 10
                }));
                continue;
            }
            futures.push_back(std::async(std::launch::async, [=, this]() mutable {
                // Run one simulation; arguments can be customized
                return this->simulate(endTime,
                                      std::forward<Observers>(observers)...); // e.g., endTime = 1000, sampleRate = 10
            }));
        }

        // Prepare for averaging
        SymbolTable<std::string, Reactant> avgReactants;
        SymbolTable<std::string, Reaction> avgReactions;

        for (auto &fut : futures)
        {
            auto [reactants, reactions] = fut.get();

            // Average reactants
            for (const auto &[name, reactant] : reactants.table)
            {
                if (avgReactants.contains(name))
                {
                    avgReactants.table.at(name).quantity += reactant.quantity;
                }
                else
                {
                    auto avgReactant = Reactant(name, reactant.quantity);
                    avgReactants.add(name, avgReactant);
                }
            }
        }

        // Divide by number of threads to get the average
        for (auto &[_, reactant] : avgReactants.table)
        {
            reactant.quantity /= numberOfThreads;
        }

        return {avgReactants, avgReactions};
    }

  private:
    std::string name;
    template <isObserver... Observers>
    auto simulate(std::size_t endTime, Observers &&...observers)
        -> std::tuple<SymbolTable<std::string, Reactant>, SymbolTable<std::string, Reaction>>
    {
        auto reactantTableAvg = SymbolTable<std::string, Reactant>{};
        auto reactionTableAvg = SymbolTable<std::string, Reaction>{};

        //
        auto reactantTableCopy = reactantTable;
        auto reactionTableCopy = reactionTable;
        double t{0};
        std::mt19937 gen(std::random_device{}());
        (..., observers(t, reactantTableCopy));
        while (t <= endTime)
        {
            Reaction r{};
            for (auto &[key, reaction] : reactionTableCopy.table)
            {
                double lambda_eff = reaction.rate;
                for (const auto &input : reaction.inputs)
                {
                    lambda_eff *= reactantTableCopy.get(input.name).quantity;
                }

                if (lambda_eff <= 0)
                {
                    // skip this reaction, or assign a large delay
                    reaction.delay = std::numeric_limits<double>::max();
                }
                else
                {
                    std::exponential_distribution<> dist(lambda_eff);
                    reaction.delay = dist(gen);
                }
                r = (reaction.delay < r.delay) ? reaction : r;
            }

            t += r.delay;

            // Execute reaction
            if (std::all_of(r.inputs.begin(), r.inputs.end(),
                            [&](const auto &input) { return reactantTableCopy.get(input.name).quantity > 0; }))
            {
                for (auto &input : r.inputs)
                {
                    reactantTableCopy.get(input.name).quantity -= 1;
                }
                for (auto &product : r.products)
                {
                    for (auto &input : product.inputs)
                    {
                        if (input.name == "Environment")
                            continue;
                        reactantTableCopy.get(input.name).quantity += 1;
                    }
                }
            }
            (..., observers(t, reactantTableCopy));
        }
        return std::tuple<SymbolTable<std::string, Reactant>, SymbolTable<std::string, Reaction>>{reactantTableCopy,
                                                                                                  reactionTableCopy};
    }
};
