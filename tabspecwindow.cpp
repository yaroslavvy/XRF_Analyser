#include "tabspecwindow.h"

TabSpecWindow::TabSpecWindow(QWidget* pwgt)
    :QTabWidget(pwgt)
{
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentChanged(int)));
}

void TabSpecWindow::slotSetXModeView(int mode)
{
   qobject_cast<WorkAreaView*>(widget(currentIndex()))->setXModeView(mode);
}

void TabSpecWindow::slotSetYModeView(int mode)
{
   qobject_cast<WorkAreaView*>(widget(currentIndex()))->setYModeView(mode);
}

void TabSpecWindow::slotCurrentChanged(int index)
{
    int xMode = qobject_cast<SpectrumChart*>(qobject_cast<WorkAreaView*>(widget(index))->chart())->getXMode();
    int yMode = qobject_cast<SpectrumChart*>(qobject_cast<WorkAreaView*>(widget(index))->chart())->getYMode();
    emit modeXChanged(xMode);
    emit modeYChanged(yMode);
}
