#ifndef QUALITATIVE_ANALYSIS_MAIN_WINDOW_H
#define QUALITATIVE_ANALYSIS_MAIN_WINDOW_H

#include <QMainWindow>
#include <single_window.h>
class QMenu;
class QMdiArea;
class QSignalMapper;
class QFocusEvent;
class QAction;

namespace ui {
    class MainWindow : public QMainWindow {

        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        void setActiveItemToolBar(bool activity);
        void setActiveCopyPasteButtons (bool activity);

    private:
        QMdiArea* m_pma;
        QMenu* m_pmnuWindows;
        QSignalMapper* m_psigMapper;
        ui::SingleWindow* createNewSingleWindow();

        QAction* m_actSave;
        QAction* m_actSelectAllItems;
        QAction* m_actDeselectAllItems;
        QAction* m_actInvertSelection;
        QAction* m_actShowHideItems;
        QAction* m_actItemPresentationSettings;
        QAction* m_actItemInformation;
        QAction* m_actDeleteItem;
        QAction* m_actCopyItem;
        QAction* m_actPasteItem;

    private slots:
        void slotNewSingleWindow();
        void slotLoad();
        void slotSave();
        void slotSaveAs();
        void slotSaveAll();
        void slotWindows();
        void slotAbout();
        void slotSetActiveSubWindow (QWidget* pwgt);
        void selectAll();
        void deselectAll();
        void invertSelection();
        void showHideItems();
        void itemPresentationSettings();
        void itemInformation();
        void deleteItem();
        void copyItem();
        void pasteItem();
    };
}



#endif // QUALITATIVE_ANALYSIS_MAIN_WINDOW_H
