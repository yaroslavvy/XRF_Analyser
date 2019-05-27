#ifndef SINGLEWINDOW_H
#define SINGLEWINDOW_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QSplitter>
#include <QListView>
#include <QToolBar>
#include <QAction>
#include <QLineSeries>
#include <QValueAxis>
#include <QAbstractAxis>
#include <QComboBox>
#include "workareaview.h"
#include "spectrumlistmodel.h"
#include "spectrumchart.h"
#include "tabspecwindow.h"
#include "spectrumspm.h"
#include "service.h"


class SingleWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SingleWindow(QWidget *pwgt = nullptr);

private:
    TabSpecWindow* pTab;
    QTextEdit* messageWindow;
    QListView* lstViewSpectrums;
    QListView* lstViewGates;
    QComboBox* pCBmodeX;
    QComboBox* pCBmodeY;

public slots:
    void slotLoad();
    void slotSave();
    void slotSaveAs();
    void slotAddTab();
    void slotRemoveTab();
};

#endif // SINGLEWINDOW_H
