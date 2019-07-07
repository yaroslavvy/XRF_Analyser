#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_LIST_VIEW_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_LIST_VIEW_H

#include <QListView>
class QMouseEvent;
class QMdiSubWindow;
#include "list_view_interface_item_tool_bar.h"

namespace ctrl {
    class SpectrumListModel;
}

namespace ui {
    class MainWindow;
    class SpectrumListView : public ui::ListViewInterfaceItemToolBar {

        Q_OBJECT

    public:
        SpectrumListView(QWidget *parent = nullptr);
        static const ctrl::SpectrumListModel* getSourceSpectrumListModel();

    public slots:
        void deselectAll() override;
        void invertSelection() override;
        void showHideItems() override;
        void itemPresentationSettings() override;
        void itemInformation() override;
        void deleteItem() override;
        void copyItem() override;
        void pasteItem() override;

    protected:
        void mouseDoubleClickEvent(QMouseEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;
        void dragEnterEvent (QDragEnterEvent *event) override;
        void dragMoveEvent (QDragMoveEvent *event) override;
        void dropEvent (QDropEvent *event) override;

        void focusInEvent(QFocusEvent *event) override;
        void focusOutEvent(QFocusEvent *event) override;

        void contextMenuEvent(QContextMenuEvent* event) override;

    private:
        void startDrag();
        QPoint m_ptDragPos;
        static ctrl::SpectrumListModel* m_sourceSpectrumListModel;
        static QMenu* m_contextMenu;
    };
}

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_LIST_VIEW_H
