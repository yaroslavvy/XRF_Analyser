#include "spectrum_chart.h"
#include <QColor>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QAbstractSeries>
#include <QTranslator>
#include <QCursor>
#include <QItemSelectionModel>
#include <gates_table_model.h>

ui::SpectrumChart::SpectrumChart(QGraphicsItem* parent)
    : QtCharts::QChart(parent),
      m_xMode(ui::AxisXMode::ENERGY_KEV),
      m_yMode(ui::AxisYMode::LINEAR),
      m_modeCursor(0),
      m_posMouse(0, 0),
      m_fullViewMinX(0),
      m_fullViewMaxX(0),
      m_fullViewMinY(0),
      m_fullViewMaxY(0)
{
    legend()->hide();
    setMargins(QMargins(0, 0, 0, 0));
    setTitle(tr("Spectrums"));

    m_axisSpecX = new QtCharts::QValueAxis;
    addAxis(m_axisSpecX, Qt::AlignBottom);
    m_axisSpecX->setRange(0.0, 1.0);
    m_axisSpecX->setTitleText(tr("Energy, KeV"));
    m_axisSpecX->setLabelFormat("%.2f");
    m_axisSpecX->setTickCount(11);
    m_axisSpecX->applyNiceNumbers();
    m_fullViewMinX = 0.0;
    m_fullViewMaxX = 1.0;
    connect(m_axisSpecX, SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(recoverAxisLimits()));

    QtCharts::QValueAxis* axisSpecY = new QtCharts::QValueAxis;
    addAxis(axisSpecY, Qt::AlignLeft);
    axisSpecY->setRange(0.0, 100.0);
    axisSpecY->setTitleText(tr("Intesity, cps"));
    axisSpecY->setLabelFormat("%.2f");
    axisSpecY->setTickCount(6);
    axisSpecY->applyNiceNumbers();
    m_fullViewMinY = 0.0;
    m_fullViewMaxY = 100.0;
    connect(axisSpecY, SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(recoverAxisLimits()));

    m_verticalLineCursor = new QtCharts::QLineSeries;
    m_verticalLineCursor->setPen(QPen(Qt::red, 1, Qt::DashLine));
    m_verticalLineCursor->append(0.5, 0.0);
    m_verticalLineCursor->append(0.5, 100.0);
    addSeries(m_verticalLineCursor);
    m_verticalLineCursor->attachAxis(m_axisSpecX);
    m_verticalLineCursor->attachAxis(axes(Qt::Vertical).back());

    m_horizontalLineCursor = new QtCharts::QLineSeries;
    m_horizontalLineCursor->setPen(QPen(Qt::red, 1, Qt::DashLine));
    m_horizontalLineCursor->append(0.0, 50.0);
    m_horizontalLineCursor->append(1.0, 50.0);
    addSeries(m_horizontalLineCursor);
    m_horizontalLineCursor->attachAxis(m_axisSpecX);
    m_horizontalLineCursor->attachAxis(axes(Qt::Vertical).back());

    setCursor(Qt::CrossCursor);
}

void ui::SpectrumChart::setXMode(ui::AxisXMode mode) {

    if(mode == m_xMode) {
        return;
    }

    switch (mode) {
        case ui::AxisXMode::ENERGY_KEV:
            m_axisSpecX->setRange(0, 1);
            m_axisSpecX->setTitleText(tr("Energy, KeV"));
            m_axisSpecX->setLabelFormat("%.2f");
            m_fullViewMinX = 0.0;
            m_fullViewMaxX = 1.0;
            break;
        case ui::AxisXMode::CHANNELS:
            m_axisSpecX->setRange(0, 64);
            m_axisSpecX->setTitleText(tr("Channels"));
            m_axisSpecX->setLabelFormat("%i");
            m_fullViewMinX = 0.0;
            m_fullViewMaxX = 64;
            break;
        case ui::AxisXMode::WAVE_LENGTH_NM:
            m_axisSpecX->setRange(0, 1);
            m_axisSpecX->setTitleText(tr("Wave length, nm"));
            m_axisSpecX->setLabelFormat("%.3f");
            m_fullViewMinX = 0.0;
            m_fullViewMaxX = 1.0;
            break;
        case ui::AxisXMode::WAVE_LENGTH_A:
            m_axisSpecX->setRange(0, 10);
            m_axisSpecX->setTitleText(tr("Wave length, A"));
            m_axisSpecX->setLabelFormat("%.2f");
            m_fullViewMinX = 0.0;
            m_fullViewMaxX = 10.0;
            break;
    }

    m_xMode = mode;
    slotUpdateChart(true);
}

ui::AxisXMode ui::SpectrumChart::getXMode() const {
    return m_xMode;
}

void ui::SpectrumChart::setYMode(ui::AxisYMode mode) {

    if(mode == m_yMode) {
        return;
    }

    disconnect(axes(Qt::Vertical).back(), SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(recoverAxisLimits()));
    removeAxis(axes(Qt::Vertical).back());
    switch (mode) {
        case ui::AxisYMode::LINEAR:
            addAxis(new QtCharts::QValueAxis, Qt::AlignLeft);
            axes(Qt::Vertical).back()->setRange(0, 100);
            qobject_cast<QtCharts::QValueAxis*>(axes(Qt::Vertical).back())->setLabelFormat("%.2f");
            qobject_cast<QtCharts::QValueAxis*>(axes(Qt::Vertical).back())->setTickCount(6);
            connect(axes(Qt::Vertical).back(), SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(recoverAxisLimits()));
            m_fullViewMinY = 0.0;
            m_fullViewMaxY = 100.0;
            break;
        case ui::AxisYMode::LOG:
            addAxis(new QtCharts::QLogValueAxis, Qt::AlignLeft);
            axes(Qt::Vertical).back()->setRange(MIN_POSSIBLE_LOG_Y_AXIS_VALUE, 100);
            qobject_cast<QtCharts::QLogValueAxis*>(axes(Qt::Vertical).back())->setLabelFormat("%.0E");
            connect(axes(Qt::Vertical).back(), SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(recoverAxisLimits()));
            m_fullViewMinY = MIN_POSSIBLE_LOG_Y_AXIS_VALUE;
            m_fullViewMaxY = 100.0;
            break;
    }
    axes(Qt::Vertical).back()->setTitleText(tr("Intesity, cps"));

    m_yMode = mode;

    slotUpdateChart(true);
}

ui::AxisYMode ui::SpectrumChart::getYMode() const {
    return m_yMode;
}

void ui::SpectrumChart::slotUpdateChart(bool resizeAxis) {
    removeAllSeries();

    QList<ctrl::SpectrumPenStruct> tmpSpecLst = m_modelSpec->getSpecList();//

    for (auto &specPenStruct : tmpSpecLst) {
        addSpectrum(specPenStruct, resizeAxis);
    }

    QList<ctrl::GatePen> tmpGateLst = m_modelGate->getGateList();//

    for (auto &gatePen : tmpGateLst) {
        addGate(gatePen, resizeAxis);
    }

    m_verticalLineCursor = new QtCharts::QLineSeries;
    m_verticalLineCursor->setPen(QPen(Qt::red, 1, Qt::DashLine));
    m_verticalLineCursor->append(m_posMouse.x(), m_fullViewMinY);
    m_verticalLineCursor->append(m_posMouse.x(), m_fullViewMaxY);
    addSeries(m_verticalLineCursor);
    m_verticalLineCursor->attachAxis(m_axisSpecX);
    m_verticalLineCursor->attachAxis(axes(Qt::Vertical).back());

    m_horizontalLineCursor = new QtCharts::QLineSeries;
    m_horizontalLineCursor->setPen(QPen(Qt::red, 1, Qt::DashLine));
    m_horizontalLineCursor->append(m_fullViewMinX, m_posMouse.y());
    m_horizontalLineCursor->append(m_fullViewMaxX, m_posMouse.y());
    addSeries(m_horizontalLineCursor);
    m_horizontalLineCursor->attachAxis(m_axisSpecX);
    m_horizontalLineCursor->attachAxis(axes(Qt::Vertical).back());

    if(resizeAxis) {
        m_axisSpecX->setRange(m_fullViewMinX, m_fullViewMaxX);
        axes(Qt::Vertical).back()->setRange(m_fullViewMinY, m_fullViewMaxY);
    }
}

void ui::SpectrumChart::setAndRepaintMouseCursor(const QPointF &newMousePos){
    if ((newMousePos.x() < m_fullViewMinX) || (newMousePos.x() > m_fullViewMaxX) || (newMousePos.y() < m_fullViewMinY) || (newMousePos.y() > m_fullViewMaxY)){
        return;
    }
    m_posMouse = newMousePos;
    m_verticalLineCursor->clear();
    m_verticalLineCursor->append(m_posMouse.x(), m_fullViewMinY);
    m_verticalLineCursor->append(m_posMouse.x(), m_fullViewMaxY);

    m_horizontalLineCursor->clear();
    m_horizontalLineCursor->append(m_fullViewMinX, m_posMouse.y());
    m_horizontalLineCursor->append(m_fullViewMaxX, m_posMouse.y());
}

void ui::SpectrumChart::addSpectrum(const ctrl::SpectrumPenStruct &specPenStruct, bool resizeAxis)
{
    if(!specPenStruct.visible) {
        return;
    }

    QtCharts::QLineSeries* series = new QtCharts::QLineSeries;
    series->setPen(specPenStruct.penForChart);

    int channel = 0;
    double startSpec = specPenStruct.spm.getSpectrumAttributes().energyStartSpectrum_kev;
    double step = specPenStruct.spm.getSpectrumAttributes().energyStepSpectrum_kev;

    double maxIntensity = 0.0;

    if(m_yMode == ui::AxisYMode::LOG) {
        maxIntensity = MIN_POSSIBLE_LOG_Y_AXIS_VALUE;
    }

    double minValX = 0.0;
    double maxValX = 0.0;

    double valX = 0.0;

    bool firstXValue = true;

    QVector<double> vecIntensities(specPenStruct.spm.getSpectrumAttributes().intensities);
    firstXValue = true;
    for (auto &valY : vecIntensities) {
        switch (m_xMode) {
            case ui::AxisXMode::ENERGY_KEV:
                valX = convertChannelToEnergyKeV(startSpec, channel, step);
                break;
            case ui::AxisXMode::CHANNELS:
                valX = channel;
                break;
            case ui::AxisXMode::WAVE_LENGTH_NM:
                valX = COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_NM / convertChannelToEnergyKeV(startSpec, channel, step);
                if(valX < 0) {
                    channel++;
                    continue;
                }
                break;
            case ui::AxisXMode::WAVE_LENGTH_A:
               valX = COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_A / convertChannelToEnergyKeV(startSpec, channel, step);
               if(valX < 0) {
                   channel++;
                   continue;
               }
               break;
        }
        if(firstXValue) {
            minValX = valX;
            firstXValue = false;
        }

        if(valX < minValX) {
            minValX = valX;
        }

        if(valX > maxValX) {
            maxValX = valX;
        }

        if(valY > maxIntensity) {
            maxIntensity = valY;
        }

        if((m_yMode == ui::AxisYMode::LOG) && (valY < MIN_POSSIBLE_LOG_Y_AXIS_VALUE )) {
            valY = MIN_POSSIBLE_LOG_Y_AXIS_VALUE;
        }
        series->append(valX, valY);
        channel++;
    }

    addSeries(series);
    series->attachAxis(m_axisSpecX);
    series->attachAxis(axes(Qt::Vertical).back());
    controlAxisLimits(maxIntensity, minValX, maxValX, resizeAxis);
}

void ui::SpectrumChart::addGate(const ctrl::GatePen& gatePen, bool resizeAxis) {
    if((gatePen.gate.getEnergyLowThreshhold() > gatePen.gate.getEnergyHighThreshhold()) || (gatePen.gate.getEnergyLowThreshhold() < 0)) {
        return;
    }

    QtCharts::QLineSeries* seriesLeft = new QtCharts::QLineSeries();
    seriesLeft->setPen(gatePen.penForChart);

    QtCharts::QLineSeries* seriesRight = new QtCharts::QLineSeries();
    seriesRight->setPen(gatePen.penForChart);

    double minValX = 0.0;
    double maxValX = 0.0;

    double energyStepOfActivatedSpectrum = 0.0;
    double energyStartOfActivatedSpectrum = 0.0;

    switch (m_xMode) {
        case ui::AxisXMode::ENERGY_KEV:
            minValX = gatePen.gate.getEnergyLowThreshhold();
            maxValX = gatePen.gate.getEnergyHighThreshhold();
            break;
        case ui::AxisXMode::CHANNELS:
            energyStepOfActivatedSpectrum = getModelSpectrums()->getEnergyStepOfActivatedSpectrum();
            energyStartOfActivatedSpectrum = getModelSpectrums()->getEnergyStartofActivatedSpectrum();
            minValX = (gatePen.gate.getEnergyLowThreshhold() - energyStartOfActivatedSpectrum) / energyStepOfActivatedSpectrum;
            maxValX = (gatePen.gate.getEnergyHighThreshhold() - energyStartOfActivatedSpectrum) / energyStepOfActivatedSpectrum;
            break;
        case ui::AxisXMode::WAVE_LENGTH_NM:
            minValX = COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_NM / gatePen.gate.getEnergyLowThreshhold();
            maxValX = COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_NM / gatePen.gate.getEnergyHighThreshhold();
            if ((minValX < MIN_POSSIBLE_WAVE_LENGTH_NM) || (maxValX > MAX_POSSIBLE_WAVE_LENGTH_NM)) {
                return;
            }
            break;
        case ui::AxisXMode::WAVE_LENGTH_A:
            minValX = COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_A / gatePen.gate.getEnergyLowThreshhold();
            maxValX = COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_A / gatePen.gate.getEnergyHighThreshhold();
            if ((minValX < MIN_POSSIBLE_WAVE_LENGTH_A) || (maxValX > MAX_POSSIBLE_WAVE_LENGTH_A)) {
                return;
            }
            break;
    }

    seriesLeft->append(minValX, m_fullViewMinY);
    seriesLeft->append(minValX, m_fullViewMaxY);

    seriesRight->append(maxValX, m_fullViewMinY);
    seriesRight->append(maxValX, m_fullViewMaxY);

    addSeries(seriesLeft);
    addSeries(seriesRight);

    seriesLeft->attachAxis(m_axisSpecX);
    seriesLeft->attachAxis(axes(Qt::Vertical).back());

    seriesRight->attachAxis(m_axisSpecX);
    seriesRight->attachAxis(axes(Qt::Vertical).back());

    controlAxisLimits(0, minValX, maxValX, resizeAxis);
}

void ui::SpectrumChart::setFullSizeSpectrumArea() {
    m_axisSpecX->setRange(m_fullViewMinX, m_fullViewMaxX);
    axes(Qt::Vertical).back()->setRange(m_fullViewMinY, m_fullViewMaxY);
}

void ui::SpectrumChart::wheelEvent(QGraphicsSceneWheelEvent* event) {
    double sizeFactor = 1.5;
    if(event->delta() < 0) {
        sizeFactor = 1 / sizeFactor;
    }

    const QRectF plotAreaChart (plotArea());
    const QPointF mousePos (event->scenePos());
    const double leftNewRect = (mousePos.x() - (mousePos.x() - plotAreaChart.left()) / sizeFactor);
    const double topNewRect = (mousePos.y() - (mousePos.y() - plotAreaChart.top()) / sizeFactor);
    const double widthNewRect = (plotAreaChart.right() - plotAreaChart.left()) / sizeFactor;
    const double heightNewRect = (plotAreaChart.bottom() - mousePos.y()) + ((mousePos.y() - plotAreaChart.top()) / sizeFactor);
    zoomIn(QRectF(leftNewRect, topNewRect, widthNewRect, heightNewRect));

    recoverAxisLimits();
    QtCharts::QChart::wheelEvent(event);
}

void ui::SpectrumChart::controlAxisLimits(double maxIntensity, double minValX, double maxValX, bool resizeAxis) {
    switch (m_yMode) {
        case ui::AxisYMode::LINEAR:
            if(qobject_cast<QtCharts::QValueAxis*>(axes(Qt::Vertical).back())->max() < (COEF_LINEAR_Y_AXIS_EXTEND * maxIntensity)) {
                m_fullViewMinY = 0.0;
                m_fullViewMaxY = maxIntensity * COEF_LINEAR_Y_AXIS_EXTEND;
                if(resizeAxis) {
                    axes(Qt::Vertical).back()->setMax(maxIntensity * COEF_LINEAR_Y_AXIS_EXTEND);
                }
            }
            break;
        case ui::AxisYMode::LOG:
            if(qobject_cast<QtCharts::QLogValueAxis*>(axes(Qt::Vertical).back())->max() < (COEF_LOG_Y_AXIS_EXTEND * maxIntensity)) {
                m_fullViewMinY = MIN_POSSIBLE_LOG_Y_AXIS_VALUE;
                m_fullViewMaxY = maxIntensity * COEF_LOG_Y_AXIS_EXTEND;
                if(resizeAxis) {
                    axes(Qt::Vertical).back()->setMax(maxIntensity * COEF_LOG_Y_AXIS_EXTEND);
                }
            }
            break;
    }

    if((m_axisSpecX->min() > minValX) && (minValX > 0)) {
        switch (m_xMode) {
            case ui::AxisXMode::WAVE_LENGTH_NM:
                m_fullViewMinX = std::max(minValX, MIN_POSSIBLE_WAVE_LENGTH_NM);//strict order because connect(axisSpecX, SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(recoverAxisLimits()));
                if(resizeAxis) {
                    m_axisSpecX->setMin(m_fullViewMinX);
                }
                break;

            case ui::AxisXMode::WAVE_LENGTH_A:
                m_fullViewMinX = std::max(minValX, MIN_POSSIBLE_WAVE_LENGTH_A);
                if(resizeAxis) {
                    m_axisSpecX->setMin(m_fullViewMinX);
                }
                break;

            default:
                m_fullViewMinX = minValX;
                if(resizeAxis) {
                    m_axisSpecX->setMin(minValX);
                }
                break;
        }
    }

    if(m_axisSpecX->max() < maxValX) {
        switch (m_xMode) {
            case ui::AxisXMode::WAVE_LENGTH_NM:
                m_fullViewMaxX = std::min(maxValX, MAX_POSSIBLE_WAVE_LENGTH_NM);
                if(resizeAxis) {
                    m_axisSpecX->setMax(m_fullViewMaxX);
                }
                break;

            case ui::AxisXMode::WAVE_LENGTH_A:
                m_fullViewMaxX = std::min(maxValX, MAX_POSSIBLE_WAVE_LENGTH_A);
                if(resizeAxis) {
                    m_axisSpecX->setMax(m_fullViewMaxX);
                }
                break;

            default:
                m_fullViewMaxX = maxValX;
                if(resizeAxis) {
                    m_axisSpecX->setMax(maxValX);
                }
                break;
        }
    }
}

double ui::SpectrumChart::convertChannelToEnergyKeV (double startSpec, double channel, double step) const {
    return startSpec + (channel * step);
}

void ui::SpectrumChart::recoverAxisLimits()
{
    if(m_axisSpecX->min() < m_fullViewMinX) {
        m_axisSpecX->setMin(m_fullViewMinX);
    }

    if(m_axisSpecX->max() > m_fullViewMaxX) {
        m_axisSpecX->setMax(m_fullViewMaxX);
    }

    QtCharts::QValueAxis* axisY;
    QtCharts::QLogValueAxis* axisLogY;

    switch (m_yMode) {
        case ui::AxisYMode::LINEAR:
            axisY = qobject_cast<QtCharts::QValueAxis*>(axes(Qt::Vertical).back());
            if(axisY->min() < m_fullViewMinY) {
                axisY->setMin(m_fullViewMinY);
            }

            if(axisY->max() > m_fullViewMaxY) {
                axisY->setMax(m_fullViewMaxY);
            }
            break;
        case ui::AxisYMode::LOG:
            axisLogY = qobject_cast<QtCharts::QLogValueAxis*>(axes(Qt::Vertical).back());
            if(axisLogY->min() < m_fullViewMinY) {
                axisLogY->setMin(m_fullViewMinY);
            }

            if(axisLogY->max() > m_fullViewMaxY) {
                axisLogY->setMax(m_fullViewMaxY);
            }
            break;
    }
}

void ui::SpectrumChart::setModelSpectrums(ctrl::SpectrumListModel *model) {
    m_modelSpec = model;
    slotUpdateChart(true);
    connect(m_modelSpec, SIGNAL(updateSpectrums(bool)), this, SLOT(slotUpdateChart(bool)));
}

void ui::SpectrumChart::setModelGates(ctrl::GatesTableModel* model) {
    m_modelGate = model;
    slotUpdateChart(true);
    connect(m_modelGate, SIGNAL(updateGates(bool)), this, SLOT(slotUpdateChart(bool)));
}

ctrl::SpectrumListModel* ui::SpectrumChart::getModelSpectrums() const {
    return m_modelSpec;
}

ctrl::GatesTableModel* ui::SpectrumChart::getModelGates() const {
    return m_modelGate;
}

void ui::SpectrumChart::slotCursorMode(int mode){
    switch (mode) {
        case 0:
        //TODO: make implementation
            break;
    }
}
