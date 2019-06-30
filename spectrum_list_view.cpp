#include "spectrum_list_view.h"
#include <QMouseEvent>
#include <QStyledItemDelegate>
#include <QDebug>
#include "spectrum_list_model.h"

ui::SpectrumListView::SpectrumListView(QWidget *parent)
    :QListView(parent)
{
}

void ui::SpectrumListView::mouseDoubleClickEvent(QMouseEvent *event) {
    QModelIndex index = indexAt(event->pos());
    if(index.isValid()){
        ctrl::SpectrumListModel* specModel = qobject_cast<ctrl::SpectrumListModel*>(model());
        specModel->setActivatedSpectrum(index);
    }
}
