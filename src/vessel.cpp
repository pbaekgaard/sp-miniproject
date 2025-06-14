#include "vessel.hpp"
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

void Vessel::simulate(std::size_t endTime, std::size_t sampleRate)
{
    double t{0};
    std::mt19937 gen(std::random_device{}());

    // Initialize a QLineSeries for each reactant
    std::map<std::string, QLineSeries *> seriesMap;
    for (auto &[name, reactant] : reactantTable.table)
    {
        auto *series = new QLineSeries();
        series->setName(QString::fromStdString(name));
        seriesMap[name] = series;
    }

    while (t <= endTime)
    {
        Reaction r{};
        for (auto &[key, reaction] : reactionTable.table)
        {
            double lambda_eff = reaction.rate;
            for (const auto &input : reaction.inputs)
            {
                lambda_eff *= reactantTable.get(input.name).quantity;
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
        if (sampleRate == 0 || static_cast<std::size_t>(t) % sampleRate == 0)
        {
            for (auto &[name, reactant] : reactantTable.table)
            {
                seriesMap[name]->append(t, reactant.quantity);
            }
        }

        // Execute reaction
        if (std::all_of(r.inputs.begin(), r.inputs.end(),
                        [&](const auto &input) { return reactantTable.get(input.name).quantity > 0; }))
        {
            for (auto &input : r.inputs)
            {
                reactantTable.get(input.name).quantity -= 1;
            }
            for (auto &product : r.products)
            {
                for (auto &input : product.inputs)
                {
                    reactantTable.get(input.name).quantity += 1;
                }
            }
        }
    }

    // Create chart and add series
    QChart *chart = new QChart();
    chart->setTitle("Reactant Quantities Over Time");

    for (auto &[name, series] : seriesMap)
    {
        chart->addSeries(series);
    }

    // Create axes
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Time");
    axisX->setLabelFormat("%.1f");
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Quantity");
    axisY->setLabelFormat("%.1f");
    chart->addAxis(axisY, Qt::AlignLeft);

    // Attach series to axes
    for (auto &[_, series] : seriesMap)
    {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }

    // Find min and max values for quantity
    double minQuantity = std::numeric_limits<double>::max();
    double maxQuantity = std::numeric_limits<double>::lowest();

    for (const auto &[name, series] : seriesMap)
    {
        for (const auto &point : series->points())
        {
            if (point.y() < minQuantity)
                minQuantity = point.y();
            if (point.y() > maxQuantity)
                maxQuantity = point.y();
        }
    }

    // Optionally add some padding for better visuals
    const double padding = (maxQuantity - minQuantity) * 0.1;
    minQuantity = std::max(0.0, minQuantity - padding);
    maxQuantity += padding;

    axisY->setRange(minQuantity, maxQuantity);

    // For time axis you can also set the range similarly if desired:
    axisX->setRange(0, endTime);

    // Display chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(3000, 600);
    chartView->show();
}
