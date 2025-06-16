#pragma once
#include "reactant.hpp"
#include "reaction.hpp"
#include "symboltable.hpp"
#include <string>
#include <tuple>

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
    auto runSimulations(std::size_t numberOfSimulations, std::size_t endTime)
        -> std::tuple<SymbolTable<std::string, Reactant>, SymbolTable<std::string, Reaction>>;

  private:
    std::string name;
    auto simulate(std::size_t endTime)
        -> std::tuple<SymbolTable<std::string, Reactant>, SymbolTable<std::string, Reaction>>;
};
