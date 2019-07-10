#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_CHART_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_CHART_H

#include <QChart>
#include <QValueAxis>
#include <QLineSeries>
#include "spectrum_pen_struct.h"
#include "spectrum_list_model.h"
#include "spectrum_chart_axis_modes.h"
#include "gate_pen.h"

namespace ctrl {
    class GatesTableModel;
}

namespace ui {
    class SpectrumChart : public QtCharts::QChart {

        Q_OBJECT

    public:
        SpectrumChart(QGraphicsItem* parent = nullptr);
        void addSpectrum(const ctrl::SpectrumPenStruct& specPenStruct, bool resizeAxis = true);
        void addGate(const ctrl::GatePen& gatePen, bool resizeAxis = false);
        void setModelSpectrums(ctrl::SpectrumListModel* model);
        void setModelGates(ctrl::GatesTableModel* model);
        void setFullSizeSpectrumArea();

        ctrl::SpectrumListModel* getModelSpectrums() const;
        ctrl::GatesTableModel* getModelGates() const;

        void setXMode(AxisXMode);
        ui::AxisXMode getXMode() const;
        void setYMode(AxisYMode);
        ui::AxisYMode getYMode() const;

    public slots:
        void recoverAxisLimits();
        void slotUpdateChart(bool resizeAxis);
        void slotCursorMode(int cursorMode);//TODO: enum for cursorMode
        void setAndRepaintMouseCursor(const QPointF &newMousePos);

    protected:
        void wheelEvent(QGraphicsSceneWheelEvent* event) override;

    private:
        void controlAxisLimits(double maxIntensity, double minValX, double maxValX, bool resizeAxis = true);
        double convertChannelToEnergyKeV (double startSpec, double channel, double step) const;

        const double MIN_POSSIBLE_LOG_Y_AXIS_VALUE = 0.00001;//min value of the log axisY can not be ZERO or NEGATIVE
        const double COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_NM = 1.239842;
        const double COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_A = 12.39842;
        const double COEF_LINEAR_Y_AXIS_EXTEND = 1.1;
        const double COEF_LOG_Y_AXIS_EXTEND = 100;
        const double MIN_POSSIBLE_WAVE_LENGTH_NM = 0.02478;
        const double MIN_POSSIBLE_WAVE_LENGTH_A = 0.2478;
        const double MAX_POSSIBLE_WAVE_LENGTH_NM = 2.4796;
        const double MAX_POSSIBLE_WAVE_LENGTH_A = 24.796;

        ctrl::SpectrumListModel* m_modelSpec;
        ctrl::GatesTableModel* m_modelGate;

        AxisXMode m_xMode;
        AxisYMode m_yMode;

        int m_modeCursor;
        QPointF m_posMouse;

        double m_fullViewMinX;
        double m_fullViewMaxX;
        double m_fullViewMinY;
        double m_fullViewMaxY;

        QtCharts::QLineSeries* m_verticalLineCursor;
        QtCharts::QLineSeries* m_horizontalLineCursor;

        QtCharts::QValueAxis* m_axisSpecX;
    };
}

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_CHART_H
