#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_CHART_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_CHART_H

#include <QChart>
#include <QValueAxis>
#include <QLineSeries>
#include "spectrum_pen_struct.h"
#include "spectrum_list_model.h"
#include "gate_pen.h"

class QItemSelectionModel;
class QLabel;

namespace ctrl {
    class GatesTableModel;
}

namespace ui {
    enum CursorMode {
        USIAL,
        SELECT_GATE_THRESHHOLDS,
        SEARCHING_ELEMENT_LINE
    };

    enum AxisXMode{
        ENERGY_KEV,
        CHANNELS,
        WAVE_LENGTH_NM,
        WAVE_LENGTH_A
    };

    enum AxisYMode{
        LINEAR,
        LOG
    };

    const double COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_NM = 1.239842;
    const double COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_A = 12.39842;
    const double MIN_POSSIBLE_WAVE_LENGTH_NM = 0.02478;
    const double MIN_POSSIBLE_WAVE_LENGTH_A = 0.2478;
    const double MAX_POSSIBLE_WAVE_LENGTH_NM = 2.4796;
    const double MAX_POSSIBLE_WAVE_LENGTH_A = 24.796;

    class SpectrumChart : public QtCharts::QChart {

        Q_OBJECT

    public:
        SpectrumChart(QGraphicsItem* parent = nullptr);
        void addSpectrum(const ctrl::SpectrumPenStruct& specPenStruct, bool resizeAxis = true);
        void addGate(const ctrl::GatePen& gatePen, bool resizeAxis = false);
        void setFullSizeSpectrumArea();
        void setCursorMode(CursorMode mode);
        void setStartEnergyGateThreshhold(double startEnergyGateThreshhold);
        void setFinishEnergyGateThreshhold(double finishEnergyGateThreshhold);

        ctrl::SpectrumListModel* getModelSpectrums() const;
        ctrl::GatesTableModel* getModelGates() const;
        QItemSelectionModel* getSelectionModelSpectrums() const;
        QItemSelectionModel* getSelectionModelGates() const;
        ui::AxisXMode getAxisXMode() const;

        double getStartEnergyGateThreshhold() const;
        double getFinishEnergyGateThreshhold() const;

        void setXMode(AxisXMode);
        ui::AxisXMode getXMode() const;
        void setYMode(AxisYMode);
        ui::AxisYMode getYMode() const;

        bool mouseIsInTheChartArea(const QPointF &newMousePos) const;

    signals:
        void setGatesThreshholds(double gateLowThreshhold, double gateHighThreshhold, int selectedRow);

    public slots:
        void recoverAxisLimits();
        void slotUpdateChart(bool resizeAxis);
        void slotUpdateChartWithoutResizeAxis();
        void slotCursorMode(int cursorMode);//TODO: enum for cursorMode
        void setAndRepaintMouseCursor(const QPointF &newMousePos);
        void setActivatedSpectrum(const ctrl::SpectrumSPM);

    protected:
        void wheelEvent(QGraphicsSceneWheelEvent* event) override;

    private:
        void controlAxisLimits(double maxIntensity, double minValX, double maxValX, bool resizeAxis = true);

        const double MIN_POSSIBLE_LOG_Y_AXIS_VALUE = 0.00001;//min value of the log axisY can not be ZERO or NEGATIVE

        const double COEF_LINEAR_Y_AXIS_EXTEND = 1.1;
        const double COEF_LOG_Y_AXIS_EXTEND = 100;

        ctrl::SpectrumListModel* m_modelSpec;
        ctrl::GatesTableModel* m_modelGate;
        QItemSelectionModel* m_selectionModelSpec;
        QItemSelectionModel* m_selectionModelGate;

        AxisXMode m_xMode;
        AxisYMode m_yMode;

        CursorMode m_modeCursor;

        double m_fullViewMinX;
        double m_fullViewMaxX;
        double m_fullViewMinY;
        double m_fullViewMaxY;

        QPointF m_mousePos;

        QtCharts::QLineSeries* m_verticalLineCursor;
        QtCharts::QLineSeries* m_horizontalLineCursor;

        QtCharts::QLineSeries* m_gateLowThreshhold;
        QtCharts::QLineSeries* m_gateHighThreshhold;

        QtCharts::QValueAxis* m_axisSpecX;

        double m_startEnergyGateThreshhold;
        double m_finishEnergyGateThreshhold;
    };
}

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_CHART_H
