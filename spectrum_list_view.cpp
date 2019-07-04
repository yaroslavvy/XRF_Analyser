#include "spectrum_list_view.h"
#include <QMouseEvent>
#include <QStyledItemDelegate>
#include <QDebug>
#include <QApplication>
#include <QDrag>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMainWindow>
#include <QItemSelectionModel>
#include "spectrum_list_model.h"
#include "spectrum_list_mime_data.h"
#include "spectrum_pen_struct.h"
#include "main_window.h"
#include "single_window.h"
#include "service.h"

ctrl::SpectrumListModel* ui::SpectrumListView::m_sourceSpectrumListModel = nullptr;

ui::SpectrumListView::SpectrumListView(QWidget *parent)
    :QListView(parent)
{
    setAcceptDrops(true);
}

const ctrl::SpectrumListModel* ui::SpectrumListView::getSourceSpectrumListModel() {
    return m_sourceSpectrumListModel;
}

void ui::SpectrumListView::deselectAll() {
    selectionModel()->clearSelection();
}

void ui::SpectrumListView::invertSelection() {
    QItemSelectionModel* spectrumListViewSelectionModel = selectionModel();

    int i = 0;
    QModelIndex index = model()->index(i++, 0);
    while (index.isValid()) {
        spectrumListViewSelectionModel->select(index, QItemSelectionModel::Toggle);
        index = model()->index(i++, 0);
    }
}

void ui::SpectrumListView::showHideSpectrum() {
    QModelIndexList indexListViewSelection = selectedIndexes();
    ctrl::SpectrumListModel* specModel = qobject_cast<ctrl::SpectrumListModel*>(model());

    for(auto &index : indexListViewSelection){
        specModel->changeVisibilitySpectrum(index);
    }
}

void ui::SpectrumListView::spectrumSettings() {

}

void ui::SpectrumListView::spectrumInformation() {

}

void ui::SpectrumListView::deleteSpectrum() {

}

void ui::SpectrumListView::mouseDoubleClickEvent(QMouseEvent *event) {
    QModelIndex index = indexAt(event->pos());
    if(index.isValid()){
        ctrl::SpectrumListModel* specModel = qobject_cast<ctrl::SpectrumListModel*>(model());
        specModel->setActivatedSpectrum(index);
    }

    QListView::mouseDoubleClickEvent(event);
}

void ui::SpectrumListView::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton){
        m_ptDragPos = event->pos();
    }
    QListView::mousePressEvent(event);
}

void ui::SpectrumListView::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton){
        int distance = (event->pos() - m_ptDragPos).manhattanLength();
        if (distance > QApplication::startDragDistance()){
            startDrag();
        }
    }
}

void ui::SpectrumListView::dragEnterEvent (QDragEnterEvent *event) {
    if(event->mimeData()->hasFormat(ctrl::SpectrumListMimeData::mimeType())){
        event->acceptProposedAction();
    }
}

void ui::SpectrumListView::dropEvent (QDropEvent *event) {
    ctrl::SpectrumListModel* thisSpectrumListModel = qobject_cast<ctrl::SpectrumListModel*>(model());
    if (m_sourceSpectrumListModel == thisSpectrumListModel) {
        return;
    }

    if (event->proposedAction() == Qt::MoveAction) {
            event->acceptProposedAction();
    } else if (event->proposedAction() == Qt::CopyAction) {
        event->acceptProposedAction();
    } else {
        return;
    }

    const ctrl::SpectrumListMimeData* spectrumListMimeData = dynamic_cast<const ctrl::SpectrumListMimeData*>(event->mimeData());
    if(spectrumListMimeData){
        QList<ctrl::SpectrumSPM> spectrumList(spectrumListMimeData->getSpectrumList());
        for(auto &spectrum : spectrumList){
            thisSpectrumListModel->addSpectrum(spectrum);
        }
    }
}

void ui::SpectrumListView::dragMoveEvent (QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void ui::SpectrumListView::startDrag() {
    const QModelIndexList indexList(selectedIndexes());
    if (indexList.isEmpty()){
        return;
    }
    const ctrl::SpectrumListModel* specModel = qobject_cast<ctrl::SpectrumListModel*>(model());
    const QList<ctrl::SpectrumPenStruct> spectrumPenStructList = specModel->getSpecList();
    QList<ctrl::SpectrumSPM> spectrumList;
    for(auto &index : indexList){
        spectrumList.push_back(spectrumPenStructList.at(index.row()).spm);
    }
    QDrag* drag = new QDrag(this);
    ctrl::SpectrumListMimeData* spectrumListMimeData = new ctrl::SpectrumListMimeData();
    spectrumListMimeData->setSpectrumList(spectrumList);
    m_sourceSpectrumListModel = qobject_cast<ctrl::SpectrumListModel*>(model());
    drag->setMimeData(spectrumListMimeData);
    drag->exec(Qt::MoveAction);
}
