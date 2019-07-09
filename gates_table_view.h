#ifndef QUALITATIVE_ANALYSIS_GATES_TABLE_VIEW_H
#define QUALITATIVE_ANALYSIS_GATES_TABLE_VIEW_H

#include <QTableView>
class QMouseEvent;
class QDragEnterEvent;
class QFocusEvent;
class QContextMenuEvent;
class QMenu;
class QPoint;
class QAction;
#include "table_view_interface_item_tool_bar.h"

namespace ctrl {
    class GatesTableModel;
}

namespace ui {
    class GatesTableView : public ui::TableViewInterfaceItemToolBar {

        Q_OBJECT

    public:
        GatesTableView(QWidget *parent = nullptr);
        static const ctrl::GatesTableModel* getSourceGateTableModel();

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
        static ctrl::GatesTableModel* m_sourceGateTableModel;
        static QMenu* m_contextMenu;

        QAction* m_actContextSelectAllItems;
        QAction* m_actContextDeselectAllItems;
        QAction* m_actContexInvertSelection;
        QAction* m_actContextShowHideItems;
        QAction* m_actContextItemPresentationSettings;
        QAction* m_actContextItemInformation;
        QAction* m_actContextDeleteItem;
        QAction* m_actContextCopyItem;
        QAction* m_actContextPasteItem;
    };
}

#endif // QUALITATIVE_ANALYSIS_GATES_TABLE_VIEW_H
