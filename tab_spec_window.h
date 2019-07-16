#ifndef QUALITATIVE_ANALYSIS_TAB_SPEC_WINDOW_H
#define QUALITATIVE_ANALYSIS_TAB_SPEC_WINDOW_H

#include <QTabWidget>
#include "work_area_view.h"

namespace ui {
    class TabSpecWindow : public QTabWidget {
        Q_OBJECT
    public:
        TabSpecWindow(QWidget* parent = nullptr);
        ui::WorkAreaView* getCurrentWorkAreaView() const;

    public slots:
        void slotSetXModeView(int mode) const;
        void slotSetYModeView(int mode) const;
        void slotCurrentChanged();

    signals:
        void modeXChanged(int mode);
        void modeYChanged(int mode);
        void modelsChartChanged();
    };
}

#endif // QUALITATIVE_ANALYSIS_TAB_SPEC_WINDOW_H
