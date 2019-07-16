#include <QWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QGraphicsScene>
#include <QDebug>
#include <QApplication>
#include <QClipboard>
#include <QLabel>
#include <QPalette>
#include "main_window.h"
#include "work_area_view.h"
#include "service.h"
#include "spectrum_list_mime_data.h"
#include "spectrum_list_view.h"
#include "gate_table_mime_data.h"
#include "gates_table_view.h"

ui::WorkAreaView::WorkAreaView(QWidget* parent)
    : QtCharts::QChartView (parent),
      m_coordinatesLabel(nullptr)
{
    setRubberBand(QtCharts::QChartView::RectangleRubberBand);
    setMouseTracking(true);
    m_coordinatesLabel = new QLabel(this);

    m_coordinatesLabel->setMinimumWidth(160);
    m_coordinatesLabel->setMinimumHeight(25);
    m_coordinatesLabel->setAlignment(Qt::AlignCenter);
    m_coordinatesLabel->setAutoFillBackground(true);

    QPalette labelPalette;
    labelPalette.setColor(QPalette::Window, QColor(255, 255, 0, 50));
    m_coordinatesLabel->setPalette(labelPalette);
    m_coordinatesLabel->hide();
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
    const QPointF mouseWidgetPos(event->localPos());
    const QPoint mouseViewPos(static_cast<int>(mouseWidgetPos.x()), static_cast<int>(mouseWidgetPos.y()));
    const QPointF mousePos(chart()->mapToValue(chart()->mapFromScene(mapToScene(mouseViewPos))));

    if ((event->modifiers() == Qt::ShiftModifier) && (event->buttons() & Qt::LeftButton)){
        getSpectrumChart()->setCursorMode(CursorMode::SELECT_GATE_THRESHHOLDS);
    }
    else {
        getSpectrumChart()->setCursorMode(CursorMode::USIAL);
    }
    getSpectrumChart()->setAndRepaintMouseCursor(mousePos);

    if(getSpectrumChart()->mouseIsInTheChartArea(mousePos)){
        QString chartXMode;
        switch (getSpectrumChart()->getXMode()) {
            case (ui::AxisXMode::ENERGY_KEV):
                chartXMode = " KeV ";
                break;
            case (ui::AxisXMode::CHANNELS):
                chartXMode = " ch ";
                break;
            case (ui::AxisXMode::WAVE_LENGTH_NM):
                chartXMode = " nm ";
                break;
            case (ui::AxisXMode::WAVE_LENGTH_A):
                chartXMode = " A ";
                break;
        }
        const QPoint labelViewPos(mouseViewPos.x() + 20, mouseViewPos.y() + 20);
        m_coordinatesLabel->move(labelViewPos);
        m_coordinatesLabel->setText(QString::number(mousePos.x()) + chartXMode + QString::number(mousePos.y()) + " cps");
        m_coordinatesLabel->show();
    }
    else {
        m_coordinatesLabel->hide();
    }

    QtCharts::QChartView::mouseMoveEvent(event);
}

void ui::WorkAreaView::mouseDoubleClickEvent(QMouseEvent* event){
    getSpectrumChart()->setFullSizeSpectrumArea();
    QtCharts::QChartView::mouseDoubleClickEvent(event);
}

void ui::WorkAreaView::mousePressEvent(QMouseEvent *event) {
    if ((event->modifiers() & Qt::ShiftModifier) && (event->buttons() & Qt::LeftButton)){
        const QPointF mouseWidgetPos(event->localPos());
        const QPoint mouseViewPos(static_cast<int>(mouseWidgetPos.x()), static_cast<int>(mouseWidgetPos.y()));
        const QPointF mousePos(chart()->mapToValue(chart()->mapFromScene(mapToScene(mouseViewPos))));
        getSpectrumChart()->setCursorMode(CursorMode::SELECT_GATE_THRESHHOLDS);
        getSpectrumChart()->setAndRepaintMouseCursor(mousePos);

        if(getSpectrumChart()->mouseIsInTheChartArea(mousePos)){
            QString chartXMode;
            switch (getSpectrumChart()->getXMode()) {
                case (ui::AxisXMode::ENERGY_KEV):
                    chartXMode = " KeV ";
                    break;
                case (ui::AxisXMode::CHANNELS):
                    chartXMode = " ch ";
                    break;
                case (ui::AxisXMode::WAVE_LENGTH_NM):
                    chartXMode = " nm ";
                    break;
                case (ui::AxisXMode::WAVE_LENGTH_A):
                    chartXMode = " A ";
                    break;
            }
            const QPoint labelViewPos(mouseViewPos.x() + 20, mouseViewPos.y() + 20);
            m_coordinatesLabel->move(labelViewPos);
            m_coordinatesLabel->setText(QString::number(mousePos.x()) + chartXMode + QString::number(mousePos.y()) + " cps");
            m_coordinatesLabel->show();
            getSpectrumChart()->setStartEnergyGateThreshhold(mousePos.x());
        }
        else {
            m_coordinatesLabel->hide();
        }
        return;
    }

    QtCharts::QChartView::mousePressEvent(event);
}

void ui::WorkAreaView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->modifiers() & Qt::ShiftModifier){
        const QPointF mouseWidgetPos(event->localPos());
        const QPoint mouseViewPos(static_cast<int>(mouseWidgetPos.x()), static_cast<int>(mouseWidgetPos.y()));
        const QPointF mousePos(chart()->mapToValue(chart()->mapFromScene(mapToScene(mouseViewPos))));

        if(getSpectrumChart()->mouseIsInTheChartArea(mousePos)){
            QString chartXMode;
            switch (getSpectrumChart()->getXMode()) {
                case (ui::AxisXMode::ENERGY_KEV):
                    chartXMode = " KeV ";
                    break;
                case (ui::AxisXMode::CHANNELS):
                    chartXMode = " ch ";
                    break;
                case (ui::AxisXMode::WAVE_LENGTH_NM):
                    chartXMode = " nm ";
                    break;
                case (ui::AxisXMode::WAVE_LENGTH_A):
                    chartXMode = " A ";
                    break;
            }
            const QPoint labelViewPos(mouseViewPos.x() + 20, mouseViewPos.y() + 20);
            m_coordinatesLabel->move(labelViewPos);
            m_coordinatesLabel->setText(QString::number(mousePos.x()) + chartXMode + QString::number(mousePos.y()) + " cps");
            m_coordinatesLabel->show();
            getSpectrumChart()->setFinishEnergyGateThreshhold(mousePos.x());
        }
        else {
            m_coordinatesLabel->hide();
        }

        getSpectrumChart()->setCursorMode(CursorMode::USIAL);
        getSpectrumChart()->setAndRepaintMouseCursor(mousePos);

        return;
    }

    QtCharts::QChartView::mouseReleaseEvent(event);
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
    getSpectrumChart()->setCursorMode(CursorMode::USIAL);
    QtCharts::QChartView::focusOutEvent(event);
}
