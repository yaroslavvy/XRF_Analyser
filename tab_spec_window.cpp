#include "tab_spec_window.h"
#include "spectrum_list_mime_data.h"
#include "spectrum_list_view.h"
#include "spectrum_list_model.h"
#include "tab_bar_spec_window.h"
#include <QDebug>
#include <QPaintEvent>

ui::TabSpecWindow::TabSpecWindow(QWidget* parent)
    :QTabWidget(parent)
{
    ui::TabBarSpecWindow* tabBarSpecWindow = new ui::TabBarSpecWindow(this);
    setTabBar(tabBarSpecWindow);
    setTabShape(TabSpecWindow::Triangular);
    setTabBarAutoHide(true);

    setAcceptDrops(true);

    setDocumentMode(true);
    setMovable(true);
    setUsesScrollButtons(true);

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
