#ifndef QUALITATIVE_ANALYSIS_TABLE_VIEW_INTERFACE_ITEM_TOOL_BAR_H
#define QUALITATIVE_ANALYSIS_TABLE_VIEW_INTERFACE_ITEM_TOOL_BAR_H

#include <QTableView>

namespace ui {
    class TableViewInterfaceItemToolBar : public QTableView {

        Q_OBJECT

    public:
        TableViewInterfaceItemToolBar(QWidget *parent = nullptr);

    public slots:
        virtual void deselectAll() = 0;
        virtual void invertSelection() = 0;
        virtual void showHideItems()  = 0;
        virtual void itemPresentationSettings() = 0;
        virtual void itemInformation() = 0;
        virtual void deleteItem() = 0;
        virtual void copyItem() = 0;
        virtual void pasteItem() = 0;
    };
}

#endif // QUALITATIVE_ANALYSIS_TABLE_VIEW_INTERFACE_ITEM_TOOL_BAR_H
