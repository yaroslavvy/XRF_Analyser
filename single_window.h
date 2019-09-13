#ifndef QUALITATIVE_ANALYSIS_SINGLE_WINDOW_H
#define QUALITATIVE_ANALYSIS_SINGLE_WINDOW_H

#include <QWidget>
class QTextEdit;
class QListView;
class QComboBox;

namespace ui {

    class SpectrumListView;

    class GatesTableView;

    class TabSpecWindow;

    class SingleWindow : public QWidget {

        Q_OBJECT

    public:
        explicit SingleWindow(QWidget *pwgt = nullptr);

    private:
        ui::TabSpecWindow* m_tab;
        QTextEdit* m_messageWindow;
        ui::SpectrumListView* m_lstViewSpectrums;
        ui::GatesTableView* m_tblViewGates;
        QComboBox* m_cBoxModeX;
        QComboBox* m_cBoxModeY;

    public slots:
        void slotLoad();
        void slotSave();
        void slotSaveAs();
        void slotAddTab();
        void slotRemoveTab();
        void slotAddGate();
        void slotUpdateViews();

    };
}

#endif // QUALITATIVE_ANALYSIS_SINGLE_WINDOW_H
