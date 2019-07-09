#include <QWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QGraphicsScene>
#include <QDebug>
#include <QApplication>
#include <QClipboard>
#include "main_window.h"
#include "work_area_view.h"
#include "service.h"
#include "spectrum_list_mime_data.h"
#include "spectrum_list_view.h"
#include "gate_table_mime_data.h"
#include "gates_table_view.h"

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
    if((event->mimeData()->hasFormat(ctrl::SpectrumListMimeData::mimeType())) || (event->mimeData()->hasFormat(ctrl::GateTableMimeData::mimeType()))) {
        event->acceptProposedAction();
    }
}

void ui::WorkAreaView::dragMoveEvent (QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void ui::WorkAreaView::dropEvent (QDropEvent *event) {
    if (event->mimeData()->hasFormat(ctrl::SpectrumListMimeData::mimeType())) {
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

    if (event->mimeData()->hasFormat(ctrl::GateTableMimeData::mimeType())) {
        //TODO: make drop implementation
    }

    bool chartIsEmpty = (getSpectrumChart()->getModelSpectrums()->rowCount() == 0);
    ui::MainWindow* mainWindow = srvcSpec::getMainWindow(this);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SELECT_ALL_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DESELECT_ALL_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::INVERT_SELECTION, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SHOW_HIDE_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_PRESENTATION_SETTINGS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_INFORMATION, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DELETE_ITEMS, !chartIsEmpty);//TODO: also it is needed to add modelGates
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::COPY_ITEMS, !chartIsEmpty);//TODO: also it is needed to add modelGates
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::PASTE_ITEMS, QApplication::clipboard()->mimeData()->hasFormat(ctrl::SpectrumListMimeData::mimeType()));
}

void ui::WorkAreaView::focusInEvent(QFocusEvent *event) {
    bool chartIsEmpty = (getSpectrumChart()->getModelSpectrums()->rowCount() == 0);
    ui::MainWindow* mainWindow = srvcSpec::getMainWindow(this);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SELECT_ALL_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DESELECT_ALL_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::INVERT_SELECTION, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SHOW_HIDE_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_PRESENTATION_SETTINGS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_INFORMATION, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DELETE_ITEMS, !chartIsEmpty);//TODO: also it is needed to add modelGates
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::COPY_ITEMS, !chartIsEmpty);//TODO: also it is needed to add modelGates
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::PASTE_ITEMS, QApplication::clipboard()->mimeData()->hasFormat(ctrl::SpectrumListMimeData::mimeType()));
    QtCharts::QChartView::focusInEvent(event);
}

void ui::WorkAreaView::focusOutEvent(QFocusEvent *event) {
    ui::MainWindow* mainWindow = srvcSpec::getMainWindow(this);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SELECT_ALL_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DESELECT_ALL_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::INVERT_SELECTION, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SHOW_HIDE_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_PRESENTATION_SETTINGS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_INFORMATION, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DELETE_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::COPY_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::PASTE_ITEMS, false);
    QtCharts::QChartView::focusInEvent(event);
}
