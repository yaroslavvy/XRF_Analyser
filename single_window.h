#ifndef QUALITATIVE_ANALYSIS_SINGLE_WINDOW_H
#define QUALITATIVE_ANALYSIS_SINGLE_WINDOW_H

#include <QWidget>
#include "tab_spec_window.h"
class QTextEdit;
class QListView;
class QComboBox;

namespace ui {
    class SingleWindow : public QWidget {

        Q_OBJECT

    public:
        explicit SingleWindow(QWidget *pwgt = nullptr);

    private:
        ui::TabSpecWindow* m_tab;
        QTextEdit* m_messageWindow;
        QListView* m_lstViewSpectrums;
        QListView* m_lstViewGates;
        QComboBox* m_cBoxModeX;
        QComboBox* m_cBoxModeY;

    public slots:
        void slotLoad();
        void slotSave();
        void slotSaveAs();
        void slotAddTab();
        void slotRemoveTab();
        void slotUpdateViews();
    };
}

#endif // QUALITATIVE_ANALYSIS_SINGLE_WINDOW_H
