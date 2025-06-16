#pragma once

#include "symboltable.hpp"
#include "reactant.hpp"
#include "reaction.hpp"
#include "charts.hpp"
#include <map>
#include <string>
#include <QtSvg/QSvgGenerator>
#include <QtCharts>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void operator()(const double t, SymbolTable<std::string, Reactant>& state) = 0;
    virtual void finalize (const double end_time) const = 0;
};
// R7
class CircadianObserver : public Observer {
public:
    CircadianObserver() = default;

    void operator() (const double t, SymbolTable<std::string, Reactant>& state) override {
        if (series_map.empty()) {
            for (const auto& [name, reactant] : state.table) {
                if (name == "Environment") continue;
                auto* series = new QLineSeries();
                series->setName(QString::fromStdString(name));
                series_map[name] = series;
            }
        }

        for (const auto& [name, reactant] : state.table) {
            if (name == "Environment") continue;
            series_map[name]->append(t, reactant.quantity);
        }
    }

    void finalize(const double end_time) const override {
    }
    SymbolTable<std::string, Reactant> peak_state;

private:
    std::map<std::string, QLineSeries*> series_map;

};

class ExampleObserver : public Observer {
public:
    ExampleObserver() = default;

    void operator()(const double t, SymbolTable<std::string, Reactant>& state) override {
        if (series_map.empty()) {
            for (const auto& [name, reactant] : state.table) {
                if (name == "Env") continue;
                auto* series = new QLineSeries();
                series->setName(QString::fromStdString(name));
                series_map[name] = series;
            }
        }

        for (const auto& [name, reactant] : state.table) {
            if (name == "Env") continue;
            series_map[name]->append(t, reactant.quantity);
        }
    }

    void finalize(const double end_time) const override {
    }
    SymbolTable<std::string, Reactant> peak_state;

private:
    std::map<std::string, QLineSeries*> series_map;

};


class CovidObserver : public Observer {
public:
    CovidObserver() = default;

    void operator()(const double t, SymbolTable<std::string, Reactant>& state) override {
        if (series_map.empty()) {
            for (const auto& [name, reactant] : state.table) {
                if (name == "Env") continue;
                auto* series = new QLineSeries();
                if (name == "H") {
                    series->setName(QString::fromStdString(name + "*1000"));
                    series_map[name] = series;
                }
                else {
                    series->setName(QString::fromStdString(name));
                    series_map[name] = series;
                }
            }
        }

        if (peak_state.table.empty()) {
            peak_state = state;
        }
        else if (state.get("H").quantity > peak_state.get("H").quantity) {
            peak_state = state;
        }

        for (const auto& [name, reactant] : state.table) {
            if (name == "Env") continue;
            if (name == "H") {
                series_map[name]->append(t, reactant.quantity*1000);
            }
            else {
                series_map[name]->append(t, reactant.quantity);
            }
        }
    }

    void finalize(const double end_time) const override {

    }
     // R7
    SymbolTable<std::string, Reactant> peak_state;

private:
    std::map<std::string, QLineSeries*> series_map;

};

class ChartObserver : public Observer {
    public:
    void operator()(const double t, SymbolTable<std::string, Reactant>& state) override {
        if (series_map.empty()) {
            // Initialize a QLineSeries for each reactant
            for (auto &[name, reactant] : state.table)
            {
                auto *series = new QLineSeries();
                series->setName(QString::fromStdString(name));
                series_map[name] = series;
            }
        }
        // Sample at regular intervals
        for (auto &[name, reactant] : state.table)
        {
            series_map[name]->append(t, reactant.quantity);
        }
    }
    void finalize(const double end_time) const override {
        generateChart(end_time, series_map);
    }
    private:
        std::map<std::string, QLineSeries*> series_map;
};