#include "tab_spec_window.h"

ui::TabSpecWindow::TabSpecWindow(QWidget* parent)
    :QTabWidget(parent) {
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentChanged()));
}

void ui::TabSpecWindow::slotSetXModeView(int mode) const {
    switch (mode) {
        case 0:
            getCurrentWorkAreaView()->setXModeView(ui::AxisXMode::ENERGY_KEV);
            break;
        case 1:
            getCurrentWorkAreaView()->setXModeView(ui::AxisXMode::CHANNELS);
            break;
        case 2:
            getCurrentWorkAreaView()->setXModeView(ui::AxisXMode::WAVE_LENGTH_NM);
            break;
        case 3:
            getCurrentWorkAreaView()->setXModeView(ui::AxisXMode::WAVE_LENGTH_A);
            break;
    }
}

void ui::TabSpecWindow::slotSetYModeView(int mode) const {
   switch (mode) {
        case 0:
            getCurrentWorkAreaView()->setYModeView(ui::AxisYMode::LINEAR);
            break;
        case 1:
            getCurrentWorkAreaView()->setYModeView(ui::AxisYMode::LOG);
            break;
   }
}

ui::WorkAreaView* ui::TabSpecWindow::getCurrentWorkAreaView() const {
    return qobject_cast<WorkAreaView*>(widget(currentIndex()));
}

void ui::TabSpecWindow::slotCurrentChanged() {
    ui::AxisXMode xMode = getCurrentWorkAreaView()->getSpectrumChart()->getXMode();
    ui::AxisYMode yMode = getCurrentWorkAreaView()->getSpectrumChart()->getYMode();

    emit modeXChanged(xMode);
    emit modeYChanged(yMode);
    emit modelsChartChanged();
}
