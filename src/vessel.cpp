#include "vessel.hpp"
#include "hash.hpp"
#include "reaction.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <string>

Vessel::Vessel(std::string vessel_name)
{
    name = vessel_name;
}

Reactant Vessel::add(std::string item, std::size_t quantity)
{
    const auto reactant = Reactant(item, quantity);
    reactantTable.add(item, reactant);
    return reactant;
}

void Vessel::add(Reaction reaction_specification)
{
    reactionTable.add(reaction_specification.hash(), reaction_specification);
}

void Vessel::generateGraph() const
{
    std::ofstream file("graph.dot");
    std::string out{"digraph {\n"};
    // Add Reactants
    for (const auto &[key, reactant] : reactantTable.table)
    {
        out += reactant.name + "[label=" + "\"" + reactant.name + "\"" +
               ",shape=\"box\",style=\"filled\",fillcolor=\"cyan\"];\n";
    }
    // Add Reactions
    auto rateI{0};
    for (const auto &[key, reaction] : reactionTable.table)
    {
        std::cout << reaction.rate << std::endl;
        auto const rateName = "r" + std::to_string(rateI);
        out += rateName + "[label=" + "\"" + std::format("{:g}", reaction.rate) + "\"" +
               ",shape=\"oval\",style=\"filled\",fillcolor=\"yellow\"];\n";
        for (const auto input : reaction.inputs)
        {
            out += input.name + " -> " + rateName + ";\n";
        }
        for (const auto product : reaction.products)
        {
            for (const auto productInput : product.inputs)
            {
                out += rateName + " -> " + productInput.name + ";\n";
            }
        }
        rateI++;
    }
    out += "}\n";
    std::cout << "graph.dot can be found in: " << std::filesystem::current_path();
    // put into file
    file << out;
    file.close();
}
