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
#include <QDebug>
#include "bar_clock.h"
#include "list_view_interface_item_tool_bar.h"

ui::MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    const QString pathIconMenuDefaultStyle("resources/pictures/menuIcons/defaultStyle/");
    QAction* actNew = new QAction(tr("New Window"), nullptr);
    actNew->setText(tr("&New Window"));
    actNew->setShortcut(QKeySequence("CTRL+N"));
    actNew->setToolTip(tr("New Window"));
    actNew->setToolTip(tr("Create a new windows for spectrums"));
    actNew->setWhatsThis(tr("Create a new windows for spectrums"));
    actNew->setIcon(QPixmap(pathIconMenuDefaultStyle + "newWindow52.png"));
    connect(actNew, SIGNAL(triggered()), this, SLOT(slotNewSingleWindow()));

    QAction* actOpen = new QAction(tr("Open file"), nullptr);
    actOpen->setText(tr("&Open..."));
    actOpen->setShortcut(QKeySequence("CTRL+O"));
    actOpen->setToolTip(tr("Open File"));
    actOpen->setToolTip(tr("Open file with spectrum or window with spectrums"));
    actOpen->setWhatsThis(tr("Open file with spectrum or window with spectrums"));
    actOpen->setIcon(QPixmap(pathIconMenuDefaultStyle + "openFile52.png"));
    connect(actOpen, SIGNAL(triggered()), this, SLOT(slotLoad()));

    m_actSave = new QAction(tr("Save"), nullptr);
    m_actSave->setText(tr("&Save"));
    m_actSave->setShortcut(QKeySequence("CTRL+S"));
    m_actSave->setToolTip(tr("Save"));
    m_actSave->setToolTip(tr("Save"));
    m_actSave->setWhatsThis(tr("Save"));
    m_actSave->setIcon(QPixmap(pathIconMenuDefaultStyle + "saveFile52.png"));
    connect(m_actSave, SIGNAL(triggered()), this, SLOT(slotSave()));

    QToolBar* tbFile = new QToolBar(tr("File Operations"));
    tbFile->addAction(actNew);
    tbFile->addAction(actOpen);
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
    mnuFile->addAction(actNew);
    mnuFile->addAction(actOpen);
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

    setActiveItemToolBar(false);
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
    ui::ListViewInterfaceItemToolBar* activeView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeView != nullptr){
        activeView->selectAll();
    }
}

void ui::MainWindow::deselectAll() {
    ui::ListViewInterfaceItemToolBar* activeView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeView != nullptr){
        activeView->deselectAll();
    }
}

void ui::MainWindow::invertSelection() {
    ui::ListViewInterfaceItemToolBar* activeView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeView != nullptr){
        activeView->invertSelection();
    }
}

void ui::MainWindow::showHideItems() {
    ui::ListViewInterfaceItemToolBar* activeView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeView != nullptr){
        activeView->showHideItems();
    }
}

void ui::MainWindow::itemPresentationSettings() {
    ui::ListViewInterfaceItemToolBar* activeView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeView != nullptr){
        activeView->itemPresentationSettings();
    }
}

void ui::MainWindow::itemInformation() {
    ui::ListViewInterfaceItemToolBar* activeView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeView != nullptr){
        activeView->itemInformation();
    }
}

void ui::MainWindow::deleteItem() {
    ui::ListViewInterfaceItemToolBar* activeView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeView != nullptr){
        activeView->deleteItem();
    }
}

void ui::MainWindow::copyItem() {
    ui::ListViewInterfaceItemToolBar* activeView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeView != nullptr){
        activeView->copyItem();
    }
}

void ui::MainWindow::pasteItem() {
    ui::ListViewInterfaceItemToolBar* activeView = qobject_cast<ui::ListViewInterfaceItemToolBar*>(m_pma->activeSubWindow()->focusWidget());
    if (activeView != nullptr) {
            activeView->pasteItem();
    }
}

void ui::MainWindow::setActiveItemToolBar(bool activity) {
    m_actSelectAllItems->setEnabled(activity);
    m_actDeselectAllItems->setEnabled(activity);
    m_actInvertSelection->setEnabled(activity);
    m_actShowHideItems->setEnabled(activity);
    m_actItemPresentationSettings->setEnabled(activity);
    m_actItemInformation->setEnabled(activity);
    m_actDeleteItem->setEnabled(activity);
    m_actCopyItem->setEnabled(activity);
    m_actPasteItem->setEnabled(activity);
}

void ui::MainWindow::setActiveCopyPasteButtons (bool activity) {
    m_actCopyItem->setEnabled(activity);
    m_actPasteItem->setEnabled(activity);
}
