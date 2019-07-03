#ifndef QUALITATIVE_ANALYSIS_TAB_BAR_SPEC_WINDOW_H
#define QUALITATIVE_ANALYSIS_TAB_BAR_SPEC_WINDOW_H

#include<QTabBar>

namespace ui {
    class TabBarSpecWindow : public QTabBar {
        Q_OBJECT
    public:
        TabBarSpecWindow(QWidget *parent = nullptr);

    protected:
        void dragEnterEvent (QDragEnterEvent *event) override;
        void dragMoveEvent (QDragMoveEvent *event) override;
        void dropEvent (QDropEvent *event) override;
    };
}


#endif // QUALITATIVE_ANALYSIS_TAB_BAR_SPEC_WINDOW_H
