#include "charttest.h"

static int randomBetween(int low, int high, int seed)
{
    qsrand(seed); // Установка базового числа для отсчёта рандома в qrand
    return (qrand() % ((high + 1) - low) + low);
}

ChartTest::ChartTest(QWidget* pwgt)
    : QWidget(pwgt)
{
        int seed = 0;
        QHBoxLayout* horizontalLayout = new QHBoxLayout;
        this->setLayout(horizontalLayout);

        for (int i = 0; i < 5; ++i)
        {
            QtCharts::QChartView *chartView = new QtCharts::QChartView(this);
            horizontalLayout->addWidget(chartView);

            seed = randomBetween(0, 100, seed);
            QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
            int k = 0;
            while (k <= 100)
            {
                *series << QPointF(sin((seed+ k)*0.1), k);
                ++k;
            }

            QtCharts::QChart *chart = new QtCharts::QChart();
            chart->addSeries(series);
            chart->legend()->hide();
            chart->setTitle("Graphic");

            chart->setToolTip(QString("График №%1\n"
                                      "Количество отсчётов %2").arg(i + 1).arg(k));

            QtCharts::QValueAxis *axisX = new QtCharts::QValueAxis();
            axisX->setTitleText("x, м");
            axisX->setLabelFormat("%i");
            axisX->setTickCount(1);
            chart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);

            QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
            axisY->setTitleText("t, мс");
            axisY->setLabelFormat("%g");
            axisY->setTickCount(5);
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);

            chartView->setChart(chart);
        }
}
