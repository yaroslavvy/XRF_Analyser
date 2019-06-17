#include "bar_clock.h"
#include <QTimer>
#include <QDateTime>

ui::BarClock::BarClock(QWidget* parent)
    : QLabel(parent) {
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(slotUpdateDateTime()));
    timer->start(200);
    slotUpdateDateTime();
}

void ui::BarClock::slotUpdateDateTime() {
    const QString str(QDateTime::currentDateTime().toString(Qt::SystemLocaleDate));
    setText("<font size = 3>" + str + "</font>\t");
}
