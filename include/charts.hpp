#pragma once
#include <QLineSeries>
#include <map>
void generateChart(std::size_t endTime, std::map<std::string, QLineSeries *> seriesMap);
