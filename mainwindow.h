#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSignalMapper>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include "singlewindow.h"
#include "barclock.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QMdiArea* m_pma;
    QMenu* m_pmnuWindows;
    QSignalMapper* m_psigMapper;
    SingleWindow* createNewSingleWindow();
private slots:
    void slotNewSingleWindow();
    void slotLoad();
    void slotSave();
    void slotSaveAs();
    void slotSaveAll();
    void slotWindows();
    void slotAbout();
    void slotSetActiveSubWindow (QWidget*);
};

#endif // MAINWINDOW_H
