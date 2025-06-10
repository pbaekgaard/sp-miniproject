#include "vessel.hpp"
#include "hash.hpp"
#include "reaction.hpp"
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <random>
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

// R2
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

void Vessel::simulate(std::size_t endTime) {
    double t{0};
    std::mt19937 gen(std::random_device{}());
    std::cout << "Before:\n" << std::endl;
    reactantTable.print();
    while (t<=endTime)
    {
        Reaction r{};
        for (auto &[key, reaction] : reactionTable.table) {
            auto lambda_eff = reaction.rate;
            for (const auto input : reaction.inputs) {
                lambda_eff *= reactantTable.get(input.name).quantity;
            }
            std::exponential_distribution<> dist(lambda_eff);
            reaction.delay = dist(gen);
            r = reaction.delay < r.delay ? reaction : r;
        }
        t += r.delay;
        if (std::all_of(r.inputs.begin(), r.inputs.end(),[&](const auto input) {return reactantTable.get(input.name).quantity > 0;})) {
            for (auto &input : r.inputs) {
                auto &reactant = reactantTable.get(input.name);
                reactant.quantity -= 1;
            }
            for (auto &product : r.products) {
                for (auto &input : product.inputs) {
                    auto &reactant = reactantTable.get(input.name);
                    reactant.quantity += 1;
                }
            }
        }
    }
    std::cout << "After:\n" << std::endl;
    reactantTable.print();
}
