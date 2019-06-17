#include "work_area_view.h"
#include <QWidget>
#include <QGraphicsScene>

ui::WorkAreaView::WorkAreaView(QWidget* parent)
    : QtCharts::QChartView (parent) {
    setRubberBand(QtCharts::QChartView::RectangleRubberBand);
    setMouseTracking(true);
}

void ui::WorkAreaView::setXModeView(ui::AxisXMode mode) {
    getSpectrumChart()->setXMode(mode);
}

void ui::WorkAreaView::setYModeView(ui::AxisYMode mode) {
    getSpectrumChart()->setYMode(mode);
}

ui::SpectrumChart* ui::WorkAreaView::getSpectrumChart()const {
    return qobject_cast<ui::SpectrumChart*>(chart());
}

void ui::WorkAreaView::mouseMoveEvent(QMouseEvent *event){
    const QPointF widgetPos(event->localPos());
    const QPoint viewPos(static_cast<int>(widgetPos.x()), static_cast<int>(widgetPos.y()));
    const QPointF mousePos(chart()->mapToValue(chart()->mapFromScene(mapToScene(viewPos))));
    getSpectrumChart()->setAndRepaintMouseCursor(mousePos);
    QtCharts::QChartView::mouseMoveEvent(event);
}

void ui::WorkAreaView::mouseDoubleClickEvent(QMouseEvent* event){
    getSpectrumChart()->setFullSizeSpectrumArea();
    QtCharts::QChartView::mouseDoubleClickEvent(event);
}
