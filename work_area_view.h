#ifndef QUALITATIVE_ANALYSIS_WORK_AREA_VIEW_H
#define QUALITATIVE_ANALYSIS_WORK_AREA_VIEW_H

#include <QChartView>
#include "spectrum_chart.h"
#include "spectrum_chart_axis_modes.h"
class QWidget;

namespace ui {
    class WorkAreaView : public QtCharts::QChartView {
        Q_OBJECT
    public:
        WorkAreaView(QWidget* parent = nullptr);
        void setXModeView(ui::AxisXMode mode);
        void setYModeView(ui::AxisYMode mode);
        ui::SpectrumChart* getSpectrumChart() const;

    protected:
        virtual void mouseMoveEvent(QMouseEvent* event) override;
        virtual void mouseDoubleClickEvent(QMouseEvent* event)override;
    };
}

#endif // QUALITATIVE_ANALYSIS_WORK_AREA_VIEW_H
