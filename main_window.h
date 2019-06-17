#ifndef QUALITATIVE_ANALYSIS_MAIN_WINDOW_H
#define QUALITATIVE_ANALYSIS_MAIN_WINDOW_H

#include <QMainWindow>
#include <single_window.h>
class QMenu;
class QMdiArea;
class QSignalMapper;

namespace ui {
    class MainWindow : public QMainWindow {

        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);

    private:
        QMdiArea* m_pma;
        QMenu* m_pmnuWindows;
        QSignalMapper* m_psigMapper;
        ui::SingleWindow* createNewSingleWindow();

    private slots:
        void slotNewSingleWindow();
        void slotLoad();
        void slotSave();
        void slotSaveAs();
        void slotSaveAll();
        void slotWindows();
        void slotAbout();
        void slotSetActiveSubWindow (QWidget* pwgt);
    };
}



#endif // QUALITATIVE_ANALYSIS_MAIN_WINDOW_H
