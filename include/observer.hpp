#pragma once

#include "charts.hpp"
#include "reactant.hpp"
#include "reaction.hpp"
#include "symboltable.hpp"
#include <QtCharts>
#include <QtSvg/QSvgGenerator>
#include <map>
#include <string>

//R7
class Observer
{
  public:
    virtual ~Observer() = default;
    virtual void operator()(const double t, SymbolTable<std::string, Reactant> &state) = 0;
    virtual void finalize(const double end_time) const = 0;
};
// R7
class PeakHospitalizedObserver : public Observer
{
  public:
    PeakHospitalizedObserver() = default;

    void operator()(const double t, SymbolTable<std::string, Reactant> &state) override;

    void finalize(const double end_time) const override;

    std::size_t peakHospitalized = 0;
};

class ChartObserver : public Observer
{
  public:
    ChartObserver(std::string _observerName)
    {
        observerName = _observerName;
    }
    void operator()(const double t, SymbolTable<std::string, Reactant> &state) override;
    void finalize(const double end_time) const override;
    std::string observerName;

  private:
    std::map<std::string, QLineSeries *> series_map;
};
