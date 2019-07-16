#ifndef QUALITATIVE_ANALYSIS_WORK_AREA_VIEW_H
#define QUALITATIVE_ANALYSIS_WORK_AREA_VIEW_H

#include <QChartView>
#include "spectrum_chart.h"
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
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseDoubleClickEvent(QMouseEvent* event)override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;

        void dragEnterEvent (QDragEnterEvent *event) override;
        void dragMoveEvent (QDragMoveEvent *event) override;
        void dropEvent (QDropEvent *event) override;

        void focusInEvent(QFocusEvent *event) override;
        void focusOutEvent(QFocusEvent *event) override;
    };
}

#endif // QUALITATIVE_ANALYSIS_WORK_AREA_VIEW_H
