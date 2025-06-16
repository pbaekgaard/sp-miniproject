#include "charts.hpp"
#include <QSvgGenerator>
#include <QtCharts>
#include <qpixmap.h>

QString findProjectRoot()
{
    QDir dir(QCoreApplication::applicationDirPath());
    while (!dir.isRoot())
    {
        if (dir.exists("CMakeLists.txt") || dir.exists(".git"))
        {
            return dir.absolutePath();
        }
        dir.cdUp();
    }
    return QCoreApplication::applicationDirPath(); // fallback
}

void generateChart(std::size_t endTime, std::map<std::string, QLineSeries *> seriesMap, std::string chartName)
{
    // Create chart and add series
    QChart *chart = new QChart();
    chart->setTitle("Reactant Quantities Over Time");
    std::replace(chartName.begin(), chartName.end(), ' ', '_');

    for (auto &[name, series] : seriesMap)
    {
        chart->addSeries(series);
        series->setName(QString::fromStdString(name)); // Ensure series names are set for legend
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
    axisX->setRange(0, endTime);

    // Display chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(1920, 1080);

    // Usage
    QString rootPath = findProjectRoot();
    chartView->grab().save(rootPath + "/" + QString::fromStdString(chartName + ".png"), "png");
    chartView->grab().save(QString::fromStdString(chartName + ".png"), "png");
    chartView->show();
}

QApplication *createApp()
{
    static int argc = 1;
    static char *argv[] = {(char *)"test", nullptr};
    static QApplication *app = new QApplication(argc, argv);
    return app;
}
