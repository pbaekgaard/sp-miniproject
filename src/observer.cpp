#include "observer.hpp"
#include "charts.hpp"
#include "reactant.hpp"
#include "reaction.hpp"
#include "symboltable.hpp"
#include <QSvgGenerator>
#include <QtCharts>
#include <map>
#include <string>

// R7
void PeakHospitalizedObserver::operator()(const double t, SymbolTable<std::string, Reactant> &state)
{
    if (state.contains("H"))
    {
        const auto hospitalized = state.get("H");
        peakHospitalized = hospitalized.quantity > peakHospitalized ? hospitalized.quantity : peakHospitalized;
    }
};

void PeakHospitalizedObserver::finalize(const double end_time) const
{
}

void ChartObserver::operator()(const double t, SymbolTable<std::string, Reactant> &state)
{
    if (series_map.empty())
    {
        // Initialize a QLineSeries for each reactant
        for (auto &[name, reactant] : state.table)
        {
            auto *series = new QLineSeries();
            if (name == "H")
            {
                series->setName(QString::fromStdString(name + "*1000"));
            }
            else
            {

                series->setName(QString::fromStdString(name));
            }
            series_map[name] = series;
        }
    }
    // Sample at regular intervals
    for (auto &[name, reactant] : state.table)
    {
        if (name == "H")
        {

            series_map[name]->append(t, reactant.quantity * 1000);
            continue;
        }
        series_map[name]->append(t, reactant.quantity);
    }
}

void ChartObserver::finalize(const double end_time) const
{
    generateChart(end_time, series_map, observerName);
}
