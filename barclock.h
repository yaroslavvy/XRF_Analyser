#ifndef BARCLOCK_H
#define BARCLOCK_H

#include <QObject>
#include <QLabel>
#include <QTimer>
#include <QDateTime>

class BarClock : public QLabel
{
    Q_OBJECT
public:
    BarClock(QWidget* pwgt = nullptr);

private slots:
    void slotUpdateDateTime();
};

#endif // BARCLOCK_H
