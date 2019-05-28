#ifndef TABSPECWINDOW_H
#define TABSPECWINDOW_H

#include <QObject>
#include <QTabWidget>
#include "workareaview.h"

class TabSpecWindow : public QTabWidget {
    Q_OBJECT
public:
    TabSpecWindow(QWidget* parent = nullptr);
public slots:
    void slotSetXModeView(int);
    void slotSetYModeView(int);
    void slotCurrentChanged(int);
signals:
    void modeXChanged(int);
    void modeYChanged(int);
};

#endif // TABSPECWINDOW_H
