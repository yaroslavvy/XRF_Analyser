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
#include <QClipboard>
#include <QApplication>
#include <QMenu>
#include <QAction>
#include "spectrum_list_model.h"
#include "spectrum_list_mime_data.h"
#include "spectrum_pen_struct.h"
#include "main_window.h"
#include "single_window.h"
#include "service.h"

ctrl::SpectrumListModel* ui::SpectrumListView::m_sourceSpectrumListModel = nullptr;
QMenu* ui::SpectrumListView::m_contextMenu = nullptr;

ui::SpectrumListView::SpectrumListView(QWidget *parent)
    :ListViewInterfaceItemToolBar(parent)
{
    const QString pathIconMenuDefaultStyle("resources/pictures/menuIcons/defaultStyle/");
    QListView::setAcceptDrops(true);

    QAction* actSelectAllItems = new QAction(tr("Select All Items"), nullptr);
    actSelectAllItems->setText(tr("&Select All Items"));
    actSelectAllItems->setShortcut(QKeySequence("CTRL+A"));
    actSelectAllItems->setToolTip(tr("Select all items in the list"));
    actSelectAllItems->setWhatsThis(tr("Select all items in the list"));
    actSelectAllItems->setIcon(QPixmap(pathIconMenuDefaultStyle + "selectAll52.png"));
    connect(actSelectAllItems, SIGNAL(triggered()), this, SLOT(selectAll()));

    QAction* actDeselectAllItems = new QAction(tr("Deselect All"), nullptr);
    actDeselectAllItems->setText(tr("&Deselect All"));
    actDeselectAllItems->setShortcut(QKeySequence("CTRL+D"));
    actDeselectAllItems->setToolTip(tr("Deselect all items in the list"));
    actDeselectAllItems->setWhatsThis(tr("Deselect all items in the list"));
    actDeselectAllItems->setIcon(QPixmap(pathIconMenuDefaultStyle + "deselectAll52.png"));
    connect(actDeselectAllItems, SIGNAL(triggered()), this, SLOT(deselectAll()));

    QAction* actInvertSelection = new QAction(tr("Invert Selection"), nullptr);
    actInvertSelection->setText(tr("&Invert Selection"));
    actInvertSelection->setShortcut(QKeySequence("CTRL+I"));
    actInvertSelection->setToolTip(tr("Invert selection of items in the list"));
    actInvertSelection->setWhatsThis(tr("Invert selection of items in the list"));
    actInvertSelection->setIcon(QPixmap(pathIconMenuDefaultStyle + "invertSelection52.png"));
    connect(actInvertSelection, SIGNAL(triggered()), this, SLOT(invertSelection()));

    QAction* actShowHideItems = new QAction(tr("Show/Hide Spectrum"), nullptr);
    actShowHideItems->setText(tr("S&how/Hide Spectrum"));
    actShowHideItems->setShortcut(QKeySequence("SPACE"));
    actShowHideItems->setToolTip(tr("Show or hide item in the chart view and list view"));
    actShowHideItems->setWhatsThis(tr("Show or hide item in the chart view and list view"));
    actShowHideItems->setIcon(QPixmap(pathIconMenuDefaultStyle + "showHideSpectrum52.png"));
    connect(actShowHideItems, SIGNAL(triggered()), this, SLOT(showHideItems()));

    QAction* actItemPresentationSettings = new QAction(tr("Item Presentation Settings"), nullptr);
    actItemPresentationSettings->setText(tr("It&em Presentation Settings"));
    actItemPresentationSettings->setShortcut(QKeySequence("CTRL+E"));
    actItemPresentationSettings->setToolTip(tr("Presentation Settings of the selected item"));
    actItemPresentationSettings->setWhatsThis(tr("Presentation Settings of the selected item"));
    actItemPresentationSettings->setIcon(QPixmap(pathIconMenuDefaultStyle + "spectrumSettings52.png"));
    connect(actItemPresentationSettings, SIGNAL(triggered()), this, SLOT(itemPresentationSettings()));

    QAction* actItemInformation = new QAction(tr("Item Information"), nullptr);
    actItemInformation->setText(tr("Item Infor&mation"));
    actItemInformation->setShortcut(QKeySequence("CTRL+M"));
    actItemInformation->setToolTip(tr("Information of the selected item"));
    actItemInformation->setWhatsThis(tr("Information of the selected item"));
    actItemInformation->setIcon(QPixmap(pathIconMenuDefaultStyle + "spectrumInformation52.png"));
    connect(actItemInformation, SIGNAL(triggered()), this, SLOT(itemInformation()));

    QAction* actDeleteItem = new QAction(tr("Delete Item"), nullptr);
    actDeleteItem->setText(tr("Delete Item"));
    actDeleteItem->setShortcut(QKeySequence("DELETE"));
    actDeleteItem->setToolTip(tr("Delete selected items"));
    actDeleteItem->setWhatsThis(tr("Delete selected items"));
    actDeleteItem->setIcon(QPixmap(pathIconMenuDefaultStyle + "delete52.png"));
    connect(actDeleteItem, SIGNAL(triggered()), this, SLOT(deleteItem()));

    m_contextMenu = new QMenu(tr("Item operations"));
    m_contextMenu->addAction(actSelectAllItems);
    m_contextMenu->addAction(actDeselectAllItems);
    m_contextMenu->addAction(actInvertSelection);
    m_contextMenu->addAction(actDeleteItem);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(actShowHideItems);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(actItemPresentationSettings);
    m_contextMenu->addAction(actItemInformation);
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

void ui::SpectrumListView::showHideItems() {
    QModelIndexList indexListViewSelection = selectedIndexes();
    ctrl::SpectrumListModel* specModel = qobject_cast<ctrl::SpectrumListModel*>(model());

    for(auto &index : indexListViewSelection){
        specModel->changeVisibilitySpectrum(index);
    }
}

void ui::SpectrumListView::itemPresentationSettings() {

}

void ui::SpectrumListView::itemInformation() {

}

void ui::SpectrumListView::deleteItem() {
    QModelIndexList indexListViewSelection = selectedIndexes();
    ctrl::SpectrumListModel* spectrumModel = qobject_cast<ctrl::SpectrumListModel*>(model());
    spectrumModel->removeSpectrum(indexListViewSelection);
    clearSelection();
    if(spectrumModel->rowCount() == 0) {
        srvcSpec::getMainWindow(this)->setActiveItemToolBar(false);
    }
}

void ui::SpectrumListView::copyItem() {
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
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->clear();
    ctrl::SpectrumListMimeData* spectrumListMimeData = new ctrl::SpectrumListMimeData();
    spectrumListMimeData->setSpectrumList(spectrumList);
    clipboard->setMimeData(spectrumListMimeData);
}

void ui::SpectrumListView::pasteItem() {
    const ctrl::SpectrumListMimeData* spectrumListMimeData = dynamic_cast<const ctrl::SpectrumListMimeData*>(QApplication::clipboard()->mimeData());
    ctrl::SpectrumListModel* thisSpectrumListModel = qobject_cast<ctrl::SpectrumListModel*>(model());
    if(spectrumListMimeData){
        QList<ctrl::SpectrumSPM> spectrumList(spectrumListMimeData->getSpectrumList());
        for(auto &spectrum : spectrumList){
            thisSpectrumListModel->addSpectrum(spectrum);
        }
    }
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

void ui::SpectrumListView::focusInEvent(QFocusEvent *event) {
    if(model()->rowCount() != 0) {
        srvcSpec::getMainWindow(this)->setActiveItemToolBar(true);
    }
    QListView::focusInEvent(event);
}

void ui::SpectrumListView::focusOutEvent(QFocusEvent *event) {
    srvcSpec::getMainWindow(this)->setActiveItemToolBar(false);
    QListView::focusOutEvent(event);
}

void ui::SpectrumListView::contextMenuEvent(QContextMenuEvent* event) {
    m_contextMenu->setEnabled(model()->rowCount() != 0);
    m_contextMenu->exec(event->globalPos());
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
