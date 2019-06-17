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
#include "bar_clock.h"

ui::MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    const QString pathIconMenuDefaultStyle("resources/pictures/menuIcons/defaultStyle/");
    QAction* actNew = new QAction(tr("New Window"), nullptr);
    actNew->setText(tr("&New Window"));
    actNew->setShortcut(QKeySequence("CTRL+N"));
    actNew->setToolTip(tr("New Window"));
    actNew->setToolTip(tr("Create a new windows for spectrums"));
    actNew->setWhatsThis(tr("Create a new windows for spectrums"));
    actNew->setIcon(QPixmap(pathIconMenuDefaultStyle + "newWindow.png"));
    connect(actNew, SIGNAL(triggered()), this, SLOT(slotNewSingleWindow()));

    QAction* actOpen = new QAction(tr("Open file"), nullptr);
    actOpen->setText(tr("&Open..."));
    actOpen->setShortcut(QKeySequence("CTRL+O"));
    actOpen->setToolTip(tr("Open File"));
    actOpen->setToolTip(tr("Open file with spectrum or window with spectrums"));
    actOpen->setWhatsThis(tr("Open file with spectrum or window with spectrums"));
    actOpen->setIcon(QPixmap(pathIconMenuDefaultStyle + "openFile.png"));
    connect(actOpen, SIGNAL(triggered()), this, SLOT(slotLoad()));

    QAction* actSave = new QAction(tr("Save"), nullptr);
    actSave->setText(tr("&Save"));
    actSave->setShortcut(QKeySequence("CTRL+S"));
    actSave->setToolTip(tr("Save"));
    actSave->setToolTip(tr("Save"));
    actSave->setWhatsThis(tr("Save"));
    actSave->setIcon(QPixmap(pathIconMenuDefaultStyle + "saveFile.png"));
    connect(actSave, SIGNAL(triggered()), this, SLOT(slotSave()));

    QToolBar* tbFile = new QToolBar(tr("File Operations"));
    tbFile->addAction(actNew);
    tbFile->addAction(actOpen);
    tbFile->addAction(actSave);
    addToolBar(Qt::LeftToolBarArea, tbFile);

    QMenu* mnuFile = new QMenu(tr("&File"));
    mnuFile->addAction(actNew);
    mnuFile->addAction(actOpen);
    mnuFile->addAction(actSave);
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
    if(pwgt) {
        m_pma->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(pwgt));
    }
}
