#pragma once
#include <QLineSeries>
#include <map>
#include <qapplication.h>
void generateChart(std::size_t endTime, std::map<std::string, QLineSeries *> seriesMap, std::string chartName);
QApplication *createApp();
