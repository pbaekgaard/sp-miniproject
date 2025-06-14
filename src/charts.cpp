#include <QtCharts>
void generateChart(std::size_t endTime, std::map<std::string, QLineSeries *> seriesMap)
{
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
