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
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setEditTriggers(QAbstractItemView::SelectedClicked);
    setAcceptDrops(true);

    m_actContextSelectAllItems = new QAction(tr("Select All Items"), nullptr);
    m_actContextSelectAllItems->setText(tr("&Select All Items"));
    m_actContextSelectAllItems->setShortcut(QKeySequence("CTRL+A"));
    m_actContextSelectAllItems->setToolTip(tr("Select all items in the list"));
    m_actContextSelectAllItems->setWhatsThis(tr("Select all items in the list"));
    m_actContextSelectAllItems->setIcon(QPixmap(pathIconMenuDefaultStyle + "selectAll52.png"));
    connect(m_actContextSelectAllItems, SIGNAL(triggered()), this, SLOT(selectAll()));

    m_actContextDeselectAllItems = new QAction(tr("Deselect All"), nullptr);
    m_actContextDeselectAllItems->setText(tr("&Deselect All"));
    m_actContextDeselectAllItems->setShortcut(QKeySequence("CTRL+D"));
    m_actContextDeselectAllItems->setToolTip(tr("Deselect all items in the list"));
    m_actContextDeselectAllItems->setWhatsThis(tr("Deselect all items in the list"));
    m_actContextDeselectAllItems->setIcon(QPixmap(pathIconMenuDefaultStyle + "deselectAll52.png"));
    connect(m_actContextDeselectAllItems, SIGNAL(triggered()), this, SLOT(deselectAll()));

    m_actContexInvertSelection = new QAction(tr("Invert Selection"), nullptr);
    m_actContexInvertSelection->setText(tr("&Invert Selection"));
    m_actContexInvertSelection->setShortcut(QKeySequence("CTRL+I"));
    m_actContexInvertSelection->setToolTip(tr("Invert selection of items in the list"));
    m_actContexInvertSelection->setWhatsThis(tr("Invert selection of items in the list"));
    m_actContexInvertSelection->setIcon(QPixmap(pathIconMenuDefaultStyle + "invertSelection52.png"));
    connect(m_actContexInvertSelection, SIGNAL(triggered()), this, SLOT(invertSelection()));

    m_actContextShowHideItems = new QAction(tr("Show/Hide Spectrum"), nullptr);
    m_actContextShowHideItems->setText(tr("S&how/Hide Spectrum"));
    m_actContextShowHideItems->setShortcut(QKeySequence("SPACE"));
    m_actContextShowHideItems->setToolTip(tr("Show or hide item in the chart view and list view"));
    m_actContextShowHideItems->setWhatsThis(tr("Show or hide item in the chart view and list view"));
    m_actContextShowHideItems->setIcon(QPixmap(pathIconMenuDefaultStyle + "showHideSpectrum52.png"));
    connect(m_actContextShowHideItems, SIGNAL(triggered()), this, SLOT(showHideItems()));

    m_actContextItemPresentationSettings = new QAction(tr("Item Presentation Settings"), nullptr);
    m_actContextItemPresentationSettings->setText(tr("It&em Presentation Settings"));
    m_actContextItemPresentationSettings->setShortcut(QKeySequence("CTRL+E"));
    m_actContextItemPresentationSettings->setToolTip(tr("Presentation Settings of the selected item"));
    m_actContextItemPresentationSettings->setWhatsThis(tr("Presentation Settings of the selected item"));
    m_actContextItemPresentationSettings->setIcon(QPixmap(pathIconMenuDefaultStyle + "spectrumSettings52.png"));
    connect(m_actContextItemPresentationSettings, SIGNAL(triggered()), this, SLOT(itemPresentationSettings()));

    m_actContextItemInformation = new QAction(tr("Item Information"), nullptr);
    m_actContextItemInformation->setText(tr("Item Infor&mation"));
    m_actContextItemInformation->setShortcut(QKeySequence("CTRL+M"));
    m_actContextItemInformation->setToolTip(tr("Information of the selected item"));
    m_actContextItemInformation->setWhatsThis(tr("Information of the selected item"));
    m_actContextItemInformation->setIcon(QPixmap(pathIconMenuDefaultStyle + "spectrumInformation52.png"));
    connect(m_actContextItemInformation, SIGNAL(triggered()), this, SLOT(itemInformation()));

    m_actContextDeleteItem = new QAction(tr("Delete Item"), nullptr);
    m_actContextDeleteItem->setText(tr("Delete Item"));
    m_actContextDeleteItem->setShortcut(QKeySequence("DELETE"));
    m_actContextDeleteItem->setToolTip(tr("Delete selected items"));
    m_actContextDeleteItem->setWhatsThis(tr("Delete selected items"));
    m_actContextDeleteItem->setIcon(QPixmap(pathIconMenuDefaultStyle + "delete52.png"));
    connect(m_actContextDeleteItem, SIGNAL(triggered()), this, SLOT(deleteItem()));

    m_actContextCopyItem = new QAction(tr("Copy Item"), nullptr);
    m_actContextCopyItem->setText(tr("Copy Item"));
    m_actContextCopyItem->setShortcut(QKeySequence("CTRL+C"));
    m_actContextCopyItem->setToolTip(tr("Copy Item"));
    m_actContextCopyItem->setWhatsThis(tr("Copy Item"));
    m_actContextCopyItem->setIcon(QPixmap(pathIconMenuDefaultStyle + "copy52.png"));
    connect(m_actContextCopyItem, SIGNAL(triggered()), this, SLOT(copyItem()));

    m_actContextPasteItem = new QAction(tr("Paste Item"), nullptr);
    m_actContextPasteItem->setText(tr("Paste Item"));
    m_actContextPasteItem->setShortcut(QKeySequence("CTRL+V"));
    m_actContextPasteItem->setToolTip(tr("Paste item"));
    m_actContextPasteItem->setWhatsThis(tr("Paste item"));
    m_actContextPasteItem->setIcon(QPixmap(pathIconMenuDefaultStyle + "paste52.png"));
    connect(m_actContextPasteItem, SIGNAL(triggered()), this, SLOT(pasteItem()));

    m_contextMenu = new QMenu(tr("Item operations"));
    m_contextMenu->addAction(m_actContextSelectAllItems);
    m_contextMenu->addAction(m_actContextDeselectAllItems);
    m_contextMenu->addAction(m_actContexInvertSelection);
    m_contextMenu->addAction(m_actContextDeleteItem);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_actContextShowHideItems);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_actContextItemPresentationSettings);
    m_contextMenu->addAction(m_actContextItemInformation);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_actContextCopyItem);
    m_contextMenu->addAction(m_actContextPasteItem);
}

const ctrl::SpectrumListModel* ui::SpectrumListView::getSourceSpectrumListModel() {
    return m_sourceSpectrumListModel;
}

void ui::SpectrumListView::deselectAll() {
    selectionModel()->clearSelection();
}

void ui::SpectrumListView::invertSelection() {
    QItemSelectionModel* spectrumListViewSelectionModel = selectionModel();
    QModelIndexList indexList;
    int rowAmount = model()->rowCount();
    for (int row = 0; row < rowAmount; ++row) {
        indexList.push_back(model()->index(row, 0));
    }
    for(auto &index : indexList){
        spectrumListViewSelectionModel->select(index, QItemSelectionModel::Toggle);
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
    MainWindow* mainWindow = srvcSpec::getMainWindow(this);
    if(spectrumModel->rowCount() == 0) {
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SELECT_ALL_ITEMS, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DESELECT_ALL_ITEMS, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::INVERT_SELECTION, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SHOW_HIDE_ITEMS, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_PRESENTATION_SETTINGS, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_INFORMATION, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DELETE_ITEMS, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::COPY_ITEMS, false);
    }
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::PASTE_ITEMS, QApplication::clipboard()->mimeData()->hasFormat(ctrl::SpectrumListMimeData::mimeType()));
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
    MainWindow* mainWindow = srvcSpec::getMainWindow(this);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::PASTE_ITEMS, QApplication::clipboard()->mimeData()->hasFormat(ctrl::SpectrumListMimeData::mimeType()));
}

void ui::SpectrumListView::pasteItem() {
    const ctrl::SpectrumListMimeData* spectrumListMimeData = dynamic_cast<const ctrl::SpectrumListMimeData*>(QApplication::clipboard()->mimeData());
    ctrl::SpectrumListModel* thisSpectrumListModel = qobject_cast<ctrl::SpectrumListModel*>(model());
    if(spectrumListMimeData != nullptr){
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

    ListViewInterfaceItemToolBar::mouseDoubleClickEvent(event);
}

void ui::SpectrumListView::mousePressEvent(QMouseEvent *event) {
    ListViewInterfaceItemToolBar::mousePressEvent(event); // should be before selectedIndexes() because otherwise selectedIndexes() = 0 always

    if(event->button() == Qt::LeftButton){
        m_ptDragPos = event->pos();
    }
    MainWindow* mainWindow = srvcSpec::getMainWindow(this);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::PASTE_ITEMS, QApplication::clipboard()->mimeData()->hasFormat(ctrl::SpectrumListMimeData::mimeType()));
    if(model()->rowCount() != 0) {
        QModelIndex index = indexAt(event->pos());
        bool isSelected = !(selectedIndexes().isEmpty()) && index.isValid();// selectedIndexes() in last coment
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SELECT_ALL_ITEMS, true);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DESELECT_ALL_ITEMS, true);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::INVERT_SELECTION, true);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SHOW_HIDE_ITEMS, isSelected);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_PRESENTATION_SETTINGS, isSelected);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_INFORMATION, isSelected);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DELETE_ITEMS, isSelected);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::COPY_ITEMS, isSelected);
    }
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
    if(spectrumListMimeData != nullptr){
        QList<ctrl::SpectrumSPM> spectrumList(spectrumListMimeData->getSpectrumList());
        for(auto &spectrum : spectrumList){
            thisSpectrumListModel->addSpectrum(spectrum);
        }
    }
    MainWindow* mainWindow = srvcSpec::getMainWindow(this);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::PASTE_ITEMS, QApplication::clipboard()->mimeData()->hasFormat(ctrl::SpectrumListMimeData::mimeType()));
    if(model()->rowCount() != 0) {
        bool isSelected = !selectedIndexes().isEmpty();
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SELECT_ALL_ITEMS, true);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DESELECT_ALL_ITEMS, true);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::INVERT_SELECTION, true);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SHOW_HIDE_ITEMS, isSelected);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_PRESENTATION_SETTINGS, isSelected);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_INFORMATION, isSelected);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DELETE_ITEMS, isSelected);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::COPY_ITEMS, isSelected);
    }
}

void ui::SpectrumListView::focusInEvent(QFocusEvent *event) {
    MainWindow* mainWindow = srvcSpec::getMainWindow(this);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::PASTE_ITEMS, QApplication::clipboard()->mimeData()->hasFormat(ctrl::SpectrumListMimeData::mimeType()));
    if(model()->rowCount() != 0) {
        bool isSelected = !selectedIndexes().isEmpty();
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SELECT_ALL_ITEMS, true);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DESELECT_ALL_ITEMS, true);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::INVERT_SELECTION, true);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SHOW_HIDE_ITEMS, isSelected);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_PRESENTATION_SETTINGS, isSelected);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_INFORMATION, isSelected);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DELETE_ITEMS, isSelected);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::COPY_ITEMS, isSelected);
    }
    ListViewInterfaceItemToolBar::focusInEvent(event);
}

void ui::SpectrumListView::focusOutEvent(QFocusEvent *event) {
    MainWindow* mainWindow = srvcSpec::getMainWindow(this);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SELECT_ALL_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DESELECT_ALL_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::INVERT_SELECTION, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SHOW_HIDE_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_PRESENTATION_SETTINGS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_INFORMATION, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DELETE_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::COPY_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::PASTE_ITEMS, false);
    ListViewInterfaceItemToolBar::focusOutEvent(event);
}

void ui::SpectrumListView::contextMenuEvent(QContextMenuEvent* event) {

    if(model()->rowCount() != 0){
        bool isSelected = !selectedIndexes().isEmpty();
        m_actContextSelectAllItems->setEnabled(true);
        m_actContextDeselectAllItems->setEnabled(true);
        m_actContexInvertSelection->setEnabled(true);
        m_actContextShowHideItems->setEnabled(isSelected);
        m_actContextItemPresentationSettings->setEnabled(isSelected);
        m_actContextItemInformation->setEnabled(isSelected);
        m_actContextDeleteItem->setEnabled(isSelected);
        m_actContextCopyItem->setEnabled(isSelected);
    }
    else {
        m_contextMenu->setEnabled(false);
    }
    m_actContextPasteItem->setEnabled(QApplication::clipboard()->mimeData()->hasFormat(ctrl::SpectrumListMimeData::mimeType()));
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

    ui::MainWindow* mainWindow = srvcSpec::getMainWindow(this);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SELECT_ALL_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DESELECT_ALL_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::INVERT_SELECTION, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SHOW_HIDE_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_PRESENTATION_SETTINGS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_INFORMATION, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DELETE_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::COPY_ITEMS, false);
    mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::PASTE_ITEMS, false);

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
