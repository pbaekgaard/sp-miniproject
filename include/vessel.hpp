#pragma once
#include "reactant.hpp"
#include "reaction.hpp"
#include "symboltable.hpp"
#include <string>

class Vessel
{
  public:
    Vessel(std::string name);
    Vessel(Vessel &&) = default;
    Vessel(const Vessel &) = default;
    Vessel &operator=(Vessel &&) = default;
    Vessel &operator=(const Vessel &) = default;
    Reactant add(std::string item, std::size_t quantity); // Adds to the symbol table
    void add(Reaction reaction_specification);        // Adds to the symbol table
    SymbolTable<std::string, Reactant> reactantTable;
    SymbolTable<std::string, Reaction> reactionTable;
    Reaction environment()
    {
        return Reaction(0);
    };
    void generateGraph() const;
    void simulate(std::size_t endTime);

  private:
    std::string name;
};
