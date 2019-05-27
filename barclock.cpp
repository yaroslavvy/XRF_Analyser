#include "barclock.h"

BarClock::BarClock(QWidget* pwgt)
    : QLabel (pwgt)
{
    QTimer* ptimer = new QTimer(this);
    connect(ptimer, SIGNAL(timeout()), SLOT(slotUpdateDateTime()));
    ptimer->start(200);
    slotUpdateDateTime();
}

void BarClock::slotUpdateDateTime()
{
    QString str = QDateTime::currentDateTime().toString(Qt::SystemLocaleDate);
    setText("<font size = 3>" + str + "</font>" + "\t");
}
