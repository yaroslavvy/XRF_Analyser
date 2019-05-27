#include "workareaview.h"

WorkAreaView::WorkAreaView(QWidget* pwgt)
    : QtCharts::QChartView (pwgt)
{
    setRubberBand(QtCharts::QChartView::RectangleRubberBand);

}

void WorkAreaView::setXModeView(int mode)
{
    qobject_cast<SpectrumChart*>(chart())->setXMode(mode);
}

void WorkAreaView::setYModeView(int mode)
{
    qobject_cast<SpectrumChart*>(chart())->setYMode(mode);
}
