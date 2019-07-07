#include <QWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QGraphicsScene>
#include <QDebug>
#include "main_window.h"
#include "work_area_view.h"
#include "service.h"
#include "spectrum_list_mime_data.h"
#include "spectrum_list_view.h"

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

void ui::WorkAreaView::mousePressEvent(QMouseEvent *event) {
    QtCharts::QChartView::mousePressEvent(event);
}

void ui::WorkAreaView::dragEnterEvent (QDragEnterEvent *event) {
    if(event->mimeData()->hasFormat(ctrl::SpectrumListMimeData::mimeType())){
        event->acceptProposedAction();
    }
}

void ui::WorkAreaView::dragMoveEvent (QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void ui::WorkAreaView::dropEvent (QDropEvent *event) {
    const ctrl::SpectrumListModel* sourceSpectrumListModel = qobject_cast<const ctrl::SpectrumListModel*>(ui::SpectrumListView::getSourceSpectrumListModel());
    ctrl::SpectrumListModel* thisSpectrumListModel = qobject_cast<ctrl::SpectrumListModel*>(getSpectrumChart()->getModelSpectrums());
    if (sourceSpectrumListModel == thisSpectrumListModel) {
        return;
    }

    if (event->proposedAction() == Qt::MoveAction) {
            event->acceptProposedAction();
    } else if (event->proposedAction() == Qt::CopyAction) {
        event->acceptProposedAction();
    } else {
        return;
    }

    const ctrl::SpectrumListMimeData* spectrumListMimeData = dynamic_cast<const ctrl::SpectrumListMimeData*>(event->mimeData());
    if(spectrumListMimeData){
        QList<ctrl::SpectrumSPM> spectrumList(spectrumListMimeData->getSpectrumList());
        for(auto &spectrum : spectrumList){
            thisSpectrumListModel->addSpectrum(spectrum);
        }
    }
}

void ui::WorkAreaView::focusInEvent(QFocusEvent *event) {
    srvcSpec::getMainWindow(this)->setActiveCopyPasteButtons(true);
    QtCharts::QChartView::focusInEvent(event);
}

void ui::WorkAreaView::focusOutEvent(QFocusEvent *event) {
    srvcSpec::getMainWindow(this)->setActiveCopyPasteButtons(false);
    QtCharts::QChartView::focusInEvent(event);
}
