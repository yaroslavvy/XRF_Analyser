#ifndef QUALITATIVE_ANALYSIS_BAR_CLOCK_H
#define QUALITATIVE_ANALYSIS_BAR_CLOCK_H

#include <QLabel>

namespace ui {
    class BarClock : public QLabel {
        Q_OBJECT
    public:
        BarClock(QWidget* parent = nullptr);

    private slots:
        void slotUpdateDateTime();
    };
}

#endif // QUALITATIVE_ANALYSIS_BAR_CLOCK_H
