#ifndef WORKAREAVIEW_H
#define WORKAREAVIEW_H

#include <QObject>
#include <QWidget>
#include <QChartView>
#include <QGraphicsScene>
#include "spectrumchart.h"

class WorkAreaView : public QtCharts::QChartView
{
    Q_OBJECT
public:
    WorkAreaView(QWidget* pwgt = nullptr);
    void setXModeView(int);
    void setYModeView(int);
};

#endif // WORKAREAVIEW_H
