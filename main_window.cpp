#include "main_window.h"
#include <QApplication>
#include <QMenu>
#include <QMdiArea>
#include <QMenuBar>
#include <QMdiSubWindow>
#include <QSignalMapper>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QTranslator>
#include <QFocusEvent>
#include <QClipboard>
#include <QDebug>
#include "bar_clock.h"
#include "list_view_interface_item_tool_bar.h"
#include "table_view_interface_item_tool_bar.h"

ui::MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    const QString pathIconMenuDefaultStyle("resources/pictures/menuIcons/defaultStyle/");
    m_actNew = new QAction(tr("New Window"), nullptr);
    m_actNew->setText(tr("&New Window"));
    m_actNew->setShortcut(QKeySequence("CTRL+N"));
    m_actNew->setToolTip(tr("New Window"));
    m_actNew->setToolTip(tr("Create a new windows for spectrums"));
    m_actNew->setWhatsThis(tr("Create a new windows for spectrums"));
    m_actNew->setIcon(QPixmap(pathIconMenuDefaultStyle + "newWindow52.png"));
    connect(m_actNew, SIGNAL(triggered()), this, SLOT(slotNewSingleWindow()));

    m_actOpen = new QAction(tr("Open file"), nullptr);
    m_actOpen->setText(tr("&Open..."));
    m_actOpen->setShortcut(QKeySequence("CTRL+O"));
    m_actOpen->setToolTip(tr("Open File"));
    m_actOpen->setToolTip(tr("Open file with spectrum or window with spectrums"));
    m_actOpen->setWhatsThis(tr("Open file with spectrum or window with spectrums"));
    m_actOpen->setIcon(QPixmap(pathIconMenuDefaultStyle + "openFile52.png"));
    connect(m_actOpen, SIGNAL(triggered()), this, SLOT(slotLoad()));

    m_actSave = new QAction(tr("Save"), nullptr);
    m_actSave->setText(tr("&Save"));
    m_actSave->setShortcut(QKeySequence("CTRL+S"));
    m_actSave->setToolTip(tr("Save"));
    m_actSave->setToolTip(tr("Save"));
    m_actSave->setWhatsThis(tr("Save"));
    m_actSave->setIcon(QPixmap(pathIconMenuDefaultStyle + "saveFile52.png"));
    connect(m_actSave, SIGNAL(triggered()), this, SLOT(slotSave()));

    QToolBar* tbFile = new QToolBar(tr("File Operations"));
    tbFile->addAction(m_actNew);
    tbFile->addAction(m_actOpen);
    tbFile->addAction(m_actSave);
    addToolBar(Qt::LeftToolBarArea, tbFile);

    m_actSelectAllItems = new QAction(tr("Select All Items"), nullptr);
    m_actSelectAllItems->setText(tr("&Select All Items"));
    m_actSelectAllItems->setShortcut(QKeySequence("CTRL+A"));
    m_actSelectAllItems->setToolTip(tr("Select all items in the list"));
    m_actSelectAllItems->setWhatsThis(tr("Select all items in the list"));
    m_actSelectAllItems->setIcon(QPixmap(pathIconMenuDefaultStyle + "selectAll52.png"));
    connect(m_actSelectAllItems, SIGNAL(triggered()), this, SLOT(selectAll()));

    m_actDeselectAllItems = new QAction(tr("Deselect All"), nullptr);
    m_actDeselectAllItems->setText(tr("&Deselect All"));
    m_actDeselectAllItems->setShortcut(QKeySequence("CTRL+D"));
    m_actDeselectAllItems->setToolTip(tr("Deselect all items in the list"));
    m_actDeselectAllItems->setWhatsThis(tr("Deselect all items in the list"));
    m_actDeselectAllItems->setIcon(QPixmap(pathIconMenuDefaultStyle + "deselectAll52.png"));
    connect(m_actDeselectAllItems, SIGNAL(triggered()), this, SLOT(deselectAll()));

    m_actInvertSelection = new QAction(tr("Invert Selection"), nullptr);
    m_actInvertSelection->setText(tr("&Invert Selection"));
    m_actInvertSelection->setShortcut(QKeySequence("CTRL+I"));
    m_actInvertSelection->setToolTip(tr("Invert selection of items in the list"));
    m_actInvertSelection->setWhatsThis(tr("Invert selection of items in the list"));
    m_actInvertSelection->setIcon(QPixmap(pathIconMenuDefaultStyle + "invertSelection52.png"));
    connect(m_actInvertSelection, SIGNAL(triggered()), this, SLOT(invertSelection()));

    m_actShowHideItems = new QAction(tr("Show/Hide Spectrum"), nullptr);
    m_actShowHideItems->setText(tr("S&how/Hide Spectrum"));
    m_actShowHideItems->setShortcut(QKeySequence("SPACE"));
    m_actShowHideItems->setToolTip(tr("Show or hide item in the chart view and list view"));
    m_actShowHideItems->setWhatsThis(tr("Show or hide item in the chart view and list view"));
    m_actShowHideItems->setIcon(QPixmap(pathIconMenuDefaultStyle + "showHideSpectrum52.png"));
    connect(m_actShowHideItems, SIGNAL(triggered()), this, SLOT(showHideItems()));

    m_actItemPresentationSettings = new QAction(tr("Item Presentation Settings"), nullptr);
    m_actItemPresentationSettings->setText(tr("It&em Presentation Settings"));
    m_actItemPresentationSettings->setShortcut(QKeySequence("CTRL+E"));
    m_actItemPresentationSettings->setToolTip(tr("Presentation Settings of the selected item"));
    m_actItemPresentationSettings->setWhatsThis(tr("Presentation Settings of the selected item"));
    m_actItemPresentationSettings->setIcon(QPixmap(pathIconMenuDefaultStyle + "spectrumSettings52.png"));
    connect(m_actItemPresentationSettings, SIGNAL(triggered()), this, SLOT(itemPresentationSettings()));

    m_actItemInformation = new QAction(tr("Item Information"), nullptr);
    m_actItemInformation->setText(tr("Item Infor&mation"));
    m_actItemInformation->setShortcut(QKeySequence("CTRL+M"));
    m_actItemInformation->setToolTip(tr("Information of the selected item"));
    m_actItemInformation->setWhatsThis(tr("Information of the selected item"));
    m_actItemInformation->setIcon(QPixmap(pathIconMenuDefaultStyle + "spectrumInformation52.png"));
    connect(m_actItemInformation, SIGNAL(triggered()), this, SLOT(itemInformation()));

    m_actDeleteItem = new QAction(tr("Delete Item"), nullptr);
    m_actDeleteItem->setText(tr("Delete Item"));
    m_actDeleteItem->setShortcut(QKeySequence("DELETE"));
    m_actDeleteItem->setToolTip(tr("Delete selected items"));
    m_actDeleteItem->setWhatsThis(tr("Delete selected items"));
    m_actDeleteItem->setIcon(QPixmap(pathIconMenuDefaultStyle + "delete52.png"));
    connect(m_actDeleteItem, SIGNAL(triggered()), this, SLOT(deleteItem()));

    QToolBar* itemToolBar = new QToolBar(tr("Item operations"));
    itemToolBar->addAction(m_actSelectAllItems);
    itemToolBar->addAction(m_actDeselectAllItems);
    itemToolBar->addAction(m_actInvertSelection);
    itemToolBar->addAction(m_actDeleteItem);
    itemToolBar->addSeparator();
    itemToolBar->addAction(m_actShowHideItems);
    itemToolBar->addSeparator();
    itemToolBar->addAction(m_actItemPresentationSettings);
    itemToolBar->addAction(m_actItemInformation);

    addToolBar(Qt::RightToolBarArea, itemToolBar);

    m_actCopyItem = new QAction(tr("Copy Item"), nullptr);
    m_actCopyItem->setText(tr("Copy Item"));
    m_actCopyItem->setShortcut(QKeySequence("CTRL+C"));
    m_actCopyItem->setToolTip(tr("Copy Item"));
    m_actCopyItem->setWhatsThis(tr("Copy Item"));
    m_actCopyItem->setIcon(QPixmap(pathIconMenuDefaultStyle + "copy52.png"));
    connect(m_actCopyItem, SIGNAL(triggered()), this, SLOT(copyItem()));

    m_actPasteItem = new QAction(tr("Paste Item"), nullptr);
    m_actPasteItem->setText(tr("Paste Item"));
    m_actPasteItem->setShortcut(QKeySequence("CTRL+V"));
    m_actPasteItem->setToolTip(tr("Paste item"));
    m_actPasteItem->setWhatsThis(tr("Paste item"));
    m_actPasteItem->setIcon(QPixmap(pathIconMenuDefaultStyle + "paste52.png"));
    connect(m_actPasteItem, SIGNAL(triggered()), this, SLOT(pasteItem()));

    QToolBar* editToolBar = new QToolBar(tr("Edit operations"));
    editToolBar->addAction(m_actCopyItem);
    editToolBar->addAction(m_actPasteItem);

    addToolBar(Qt::RightToolBarArea, editToolBar);

    QMenu* mnuFile = new QMenu(tr("&File"));
    mnuFile->addAction(m_actNew);
    mnuFile->addAction(m_actOpen);
    mnuFile->addAction(m_actSave);
    mnuFile->addAction(tr("Save &As..."), this, SLOT(slotSaveAs()));
    mnuFile->addSeparator();
    mnuFile->addAction(tr("&Quit"), qApp, SLOT(closeAllWindows()), QKeySequence("CTRL+Q"));

    menuBar()->addMenu(mnuFile);

    m_pmnuWindows = new QMenu(tr("&Windows"));
    menuBar()->addMenu(m_pmnuWindows);
    connect(m_pmnuWindows, SIGNAL(aboutToShow()), this, SLOT(slotWindows()));
    menuBar()->addSeparator();

    QMenu* mnuHelp = new QMenu(tr("&Help"));
    mnuHelp->addAction(tr("&About"), this, SLOT(slotAbout()), Qt::Key_F1);
    menuBar()->addMenu(mnuHelp);

    m_pma = new QMdiArea;
    setCentralWidget(m_pma);
    m_pma->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pma->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_psigMapper = new QSignalMapper(this);
    connect(m_psigMapper, SIGNAL(mapped(QWidget*)), this, SLOT(slotSetActiveSubWindow(QWidget*)));

    setWindowIcon(QIcon("resources/pictures/logo/logo.png"));
    setWindowTitle(tr("Qualitative analysis v1.0"));

    createNewSingleWindow()->showMaximized();

    BarClock* pStatBarClock = new BarClock;
    statusBar()->addPermanentWidget(pStatBarClock);
    statusBar()->showMessage(tr("Ready..."), 3000);

    setButtonEnable(MAIN_WINDOW_BUTTONS::SELECT_ALL_ITEMS, false);
    setButtonEnable(MAIN_WINDOW_BUTTONS::DESELECT_ALL_ITEMS, false);
    setButtonEnable(MAIN_WINDOW_BUTTONS::INVERT_SELECTION, false);
    setButtonEnable(MAIN_WINDOW_BUTTONS::SHOW_HIDE_ITEMS, false);
    setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_PRESENTATION_SETTINGS, false);
    setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_INFORMATION, false);
    setButtonEnable(MAIN_WINDOW_BUTTONS::DELETE_ITEMS, false);
    setButtonEnable(MAIN_WINDOW_BUTTONS::COPY_ITEMS, false);
    setButtonEnable(MAIN_WINDOW_BUTTONS::PASTE_ITEMS, false);

}

ui::SingleWindow* ui::MainWindow::createNewSingleWindow() {
    SingleWindow* newSingleWindow = new SingleWindow;
    m_pma->addSubWindow(newSingleWindow);
    return newSingleWindow;
}

void ui::MainWindow::slotNewSingleWindow() {
    createNewSingleWindow()->show();
    m_pma->cascadeSubWindows();
    statusBar()->showMessage(tr("New Window..."), 3000);
}

void ui::MainWindow::slotLoad()
{
    if(m_pma->activeSubWindow() == nullptr) {
        SingleWindow* pSW = createNewSingleWindow();
        pSW->slotLoad();
        pSW->showMaximized();
    }
    else {
        qobject_cast<SingleWindow*>(m_pma->activeSubWindow()->widget())->slotLoad();
    }
    statusBar()->showMessage(tr("File Open..."), 3000);
}

void ui::MainWindow::slotSave() {
    //TODO: make implementation
}

void ui::MainWindow::slotSaveAs() {
    //TODO: make implementation
}

void ui::MainWindow::slotSaveAll() {
    //TODO: make implementation
}

void ui::MainWindow::slotWindows() {
    m_pmnuWindows->clear();

    QAction* pact = m_pmnuWindows->addAction(tr("&Cascade"), m_pma, SLOT(cascadeSubWindows()));
    pact->setEnabled(!m_pma->subWindowList().isEmpty());

    pact = m_pmnuWindows->addAction(tr("&Tile"), m_pma, SLOT(tileSubWindows()));
    pact->setEnabled(!m_pma->subWindowList().isEmpty());

    m_pmnuWindows->addSeparator();

    QList<QMdiSubWindow*> listSubWindows = m_pma->subWindowList();
    for (auto &subWindow : listSubWindows){
        pact = m_pmnuWindows->addAction(subWindow->windowTitle());
        pact->setCheckable(true);
        pact->setChecked(m_pma->activeSubWindow() == subWindow);
        connect(pact, SIGNAL(triggered()), m_psigMapper, SLOT(map()));
        m_psigMapper->setMapping(pact, subWindow);
    }
}

void ui::MainWindow::slotAbout() {
    //TODO: make implementation
}

void ui::MainWindow::slotSetActiveSubWindow (QWidget* pwgt) {
    if(pwgt != nullptr) {
        m_pma->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(pwgt));
    }
}

void ui::MainWindow::selectAll() {
    ui::ListViewInterfaceItemToolBar* activeListView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeListView != nullptr){
        activeListView->selectAll();
        return;
    }
    ui::TableViewInterfaceItemToolBar* activeTableView = qobject_cast<ui::TableViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeTableView != nullptr){
        activeTableView->selectAll();
        return;
    }
}

void ui::MainWindow::deselectAll() {
    ui::ListViewInterfaceItemToolBar* activeListView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeListView != nullptr){
        activeListView->deselectAll();
    }
    ui::TableViewInterfaceItemToolBar* activeTableView = qobject_cast<ui::TableViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeTableView != nullptr){
        activeTableView->deselectAll();
        return;
    }
}

void ui::MainWindow::invertSelection() {
    ui::ListViewInterfaceItemToolBar* activeListView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeListView != nullptr){
        activeListView->invertSelection();
    }
    ui::TableViewInterfaceItemToolBar* activeTableView = qobject_cast<ui::TableViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeTableView != nullptr){
        activeTableView->invertSelection();
        return;
    }
}

void ui::MainWindow::showHideItems() {
    ui::ListViewInterfaceItemToolBar* activeListView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeListView != nullptr){
        activeListView->showHideItems();
    }
}

void ui::MainWindow::itemPresentationSettings() {
    ui::ListViewInterfaceItemToolBar* activeListView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeListView != nullptr){
        activeListView->itemPresentationSettings();
    }
}

void ui::MainWindow::itemInformation() {
    ui::ListViewInterfaceItemToolBar* activeListView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeListView != nullptr){
        activeListView->itemInformation();
    }
    ui::TableViewInterfaceItemToolBar* activeTableView = qobject_cast<ui::TableViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeTableView != nullptr){
        activeTableView->itemInformation();
        return;
    }
}

void ui::MainWindow::deleteItem() {
    ui::ListViewInterfaceItemToolBar* activeListView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeListView != nullptr){
        activeListView->deleteItem();
    }
    ui::TableViewInterfaceItemToolBar* activeTableView = qobject_cast<ui::TableViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeTableView != nullptr){
        activeTableView->deleteItem();
        return;
    }
}

void ui::MainWindow::copyItem() {
    ui::ListViewInterfaceItemToolBar* activeListView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeListView != nullptr){
        activeListView->copyItem();
    }
    ui::TableViewInterfaceItemToolBar* activeTableView = qobject_cast<ui::TableViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeTableView != nullptr){
        activeTableView->copyItem();
        return;
    }
}

void ui::MainWindow::pasteItem() {
    ui::ListViewInterfaceItemToolBar* activeListView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeListView != nullptr) {
            activeListView->pasteItem();
    }
    ui::TableViewInterfaceItemToolBar* activeTableView = qobject_cast<ui::TableViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeTableView != nullptr){
        activeTableView->pasteItem();
        return;
    }
}

void ui::MainWindow::setButtonEnable(MAIN_WINDOW_BUTTONS button, bool state) {
    switch(button) {
        case NEW_SINGLE_WINDOW:
            m_actNew->setEnabled(state);
            break;
        case OPEN:
            m_actOpen->setEnabled(state);
            break;
        case SAVE:
            m_actSave->setEnabled(state);
            break;
        case SELECT_ALL_ITEMS:
            m_actSelectAllItems->setEnabled(state);
            break;
        case DESELECT_ALL_ITEMS:
            m_actDeselectAllItems->setEnabled(state);
            break;
        case INVERT_SELECTION:
            m_actInvertSelection->setEnabled(state);
            break;
        case SHOW_HIDE_ITEMS:
            m_actShowHideItems->setEnabled(state);
            break;
        case ITEM_PRESENTATION_SETTINGS:
            m_actItemPresentationSettings->setEnabled(state);
            break;
        case ITEM_INFORMATION:
            m_actItemInformation->setEnabled(state);
            break;
        case DELETE_ITEMS:
            m_actDeleteItem->setEnabled(state);
            break;
        case COPY_ITEMS:
            m_actCopyItem->setEnabled(state);
            break;
        case PASTE_ITEMS:
            m_actPasteItem->setEnabled(state);
            break;
    }
}
