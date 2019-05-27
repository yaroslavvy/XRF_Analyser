#ifndef CHARTTEST_H
#define CHARTTEST_H

#include <QObject>
#include <QWidget>
#include <QLogValueAxis>
#include <QLineSeries>
#include <QValueAxis>
#include <QChart>
#include <QChartView>
#include <QHBoxLayout>

#include <math.h>

class ChartTest : public QWidget
{
    Q_OBJECT
public:
    explicit ChartTest(QWidget *parent = nullptr);
};

#endif // CHARTTEST_H
