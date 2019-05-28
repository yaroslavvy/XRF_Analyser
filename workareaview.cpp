#include "workareaview.h"

WorkAreaView::WorkAreaView(QWidget* parent)
    : QtCharts::QChartView (parent) {
    setRubberBand(QtCharts::QChartView::RectangleRubberBand);
    setMouseTracking(true);
}

void WorkAreaView::setXModeView(int mode) {
    qobject_cast<SpectrumChart*>(chart())->setXMode(mode);
}

void WorkAreaView::setYModeView(int mode) {
    qobject_cast<SpectrumChart*>(chart())->setYMode(mode);
}

void WorkAreaView::mouseMoveEvent(QMouseEvent *event){
    QPointF widgetPos(event->localPos());
    QPointF mousePos(chart()->mapToValue(chart()->mapFromScene(mapToScene(QPoint(static_cast<int>(widgetPos.x()), static_cast<int>(widgetPos.y()))))));
    qobject_cast<SpectrumChart*>(chart())->setAndRepaintMouseCursor(mousePos);
    QtCharts::QChartView::mouseMoveEvent(event);
}

void WorkAreaView::mouseDoubleClickEvent(QMouseEvent* event){
    qobject_cast<SpectrumChart*>(chart())->setFullSizeSpectrumArea();
    QtCharts::QChartView::mouseDoubleClickEvent(event);
}
