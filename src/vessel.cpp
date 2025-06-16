#include "vessel.hpp"
#include "charts.hpp"
#include "hash.hpp"
#include "reaction.hpp"
#include <QtCharts>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <map>
#include <random>
#include <string>
#include <thread>
#include <tuple>

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

auto Vessel::runSimulations(std::size_t numberOfThreads, std::size_t endTime)
    -> std::tuple<SymbolTable<std::string, Reactant>, SymbolTable<std::string, Reaction>>
{
    if (numberOfThreads == 1)
    {
        return simulate(endTime);
    }
    std::vector<std::future<std::tuple<SymbolTable<std::string, Reactant>, SymbolTable<std::string, Reaction>>>>
        futures;

    for (std::size_t i = 0; i < numberOfThreads; ++i)
    {
        futures.push_back(std::async(std::launch::async, [this, endTime]() {
            // Run one simulation; arguments can be customized
            return this->simulate(endTime); // e.g., endTime = 1000, sampleRate = 10
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
            auto avgReactant = Reactant(name, reactant.quantity);
            avgReactants.add(name, avgReactant);
        }
    }

    // Divide by number of threads to get the average
    for (auto &[_, reactant] : avgReactants.table)
    {
        reactant.quantity /= numberOfThreads;
    }

    return {avgReactants, avgReactions};
}

auto Vessel::simulate(std::size_t endTime)
    -> std::tuple<SymbolTable<std::string, Reactant>, SymbolTable<std::string, Reaction>>
{
    auto reactantTableAvg = SymbolTable<std::string, Reactant>{};
    auto reactionTableAvg = SymbolTable<std::string, Reaction>{};

    //
    auto reactantTableCopy = reactantTable;
    auto reactionTableCopy = reactionTable;
    double t{0};
    std::mt19937 gen(std::random_device{}());

    // Initialize a QLineSeries for each reactant
    std::map<std::string, QLineSeries *> seriesMap;
    for (auto &[name, reactant] : reactantTableCopy.table)
    {
        auto *series = new QLineSeries();
        series->setName(QString::fromStdString(name));
        seriesMap[name] = series;
    }

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

        // Sample at regular intervals
        for (auto &[name, reactant] : reactantTableCopy.table)
        {
            seriesMap[name]->append(t, reactant.quantity);
        }

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
    }
    // generateChart(endTime, seriesMap);

    return std::tuple<SymbolTable<std::string, Reactant>, SymbolTable<std::string, Reaction>>{reactantTableCopy,
                                                                                              reactionTableCopy};
}
