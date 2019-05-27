#include "mainwindow.h"

MainWindow::MainWindow(QWidget *pwgt)
    : QMainWindow(pwgt)
{
    QString pathIconMenuDefaultStyle("resources/pictures/menuIcons/defaultStyle/");
    QAction* pactNew = new QAction("New Window", nullptr);
    pactNew->setText("&New Window");
    pactNew->setShortcut(QKeySequence("CTRL+N"));
    pactNew->setToolTip("New Window");
    pactNew->setToolTip("Create a new windows for spectrums");
    pactNew->setWhatsThis("Create a new windows for spectrums");
    pactNew->setIcon(QPixmap(pathIconMenuDefaultStyle + "newWindow.png"));
    connect(pactNew, SIGNAL(triggered()), this, SLOT(slotNewSingleWindow()));

    QAction* pactOpen = new QAction("Open file", nullptr);
    pactOpen->setText("&Open...");
    pactOpen->setShortcut(QKeySequence("CTRL+O"));
    pactOpen->setToolTip("Open File");
    pactOpen->setToolTip("Open file with spectrum or window with spectrums");
    pactOpen->setWhatsThis("Open file with spectrum or window with spectrums");
    pactOpen->setIcon(QPixmap(pathIconMenuDefaultStyle + "openFile.png"));
    connect(pactOpen, SIGNAL(triggered()), this, SLOT(slotLoad()));

    QAction* pactSave = new QAction("Save", nullptr);
    pactSave->setText("&Save");
    pactSave->setShortcut(QKeySequence("CTRL+S"));
    pactSave->setToolTip("Save");
    pactSave->setToolTip("Save");
    pactSave->setWhatsThis("Save");
    pactSave->setIcon(QPixmap(pathIconMenuDefaultStyle + "saveFile.png"));
    connect(pactSave, SIGNAL(triggered()), this, SLOT(slotSave()));

    QToolBar* ptbFile = new QToolBar("File Operations");
    ptbFile->addAction(pactNew);
    ptbFile->addAction(pactOpen);
    ptbFile->addAction(pactSave);
    addToolBar(Qt::RightToolBarArea, ptbFile);

    QToolBar* ptbFile2 = new QToolBar("File Operations3");
    ptbFile2->addAction(pactNew);
    ptbFile2->addAction(pactOpen);
    ptbFile2->addAction(pactSave);
    addToolBar(Qt::LeftToolBarArea, ptbFile2);

    QMenu* pmnuFile = new QMenu("&File");
    pmnuFile->addAction(pactNew);
    pmnuFile->addAction(pactOpen);
    pmnuFile->addAction(pactSave);
    pmnuFile->addAction("Save &As...", this, SLOT(slotSaveAs()));
    pmnuFile->addSeparator();
    pmnuFile->addAction("&Quit", qApp, SLOT(closeAllWindows()), QKeySequence("CTRL+Q"));

    menuBar()->addMenu(pmnuFile);

    m_pmnuWindows = new QMenu("&Windows");
    menuBar()->addMenu(m_pmnuWindows);
    connect(m_pmnuWindows, SIGNAL(aboutToShow()), this, SLOT(slotWindows()));
    menuBar()->addSeparator();

    QMenu* pmnuHelp = new QMenu("&Help");
    pmnuHelp->addAction("&About", this, SLOT(slotAbout()), Qt::Key_F1);
    menuBar()->addMenu(pmnuHelp);

    m_pma = new QMdiArea;
    m_pma->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pma->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setCentralWidget(m_pma);

    m_psigMapper = new QSignalMapper(this);
    connect(m_psigMapper, SIGNAL(mapped(QWidget*)), this, SLOT(slotSetActiveSubWindow(QWidget*)));

    setWindowIcon(QIcon("resources/pictures/logo/logo.png"));
    setWindowTitle("Qualitative analysis v1.0");

    createNewSingleWindow()->showMaximized();

    BarClock* pStatBarClock = new BarClock;
    statusBar()->addPermanentWidget(pStatBarClock);
    statusBar()->showMessage("Ready...", 3000);
}

MainWindow::~MainWindow()
{
}

SingleWindow* MainWindow::createNewSingleWindow()
{
    SingleWindow* pSW = new SingleWindow;
    m_pma->addSubWindow(pSW);
   // m_pma->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(pSW));
    pSW->setAttribute(Qt::WA_DeleteOnClose);
    pSW->setWindowTitle("Unknown spectrum");
    return pSW;
}

void MainWindow::slotNewSingleWindow()
{
    createNewSingleWindow()->show();
    m_pma->cascadeSubWindows();
    statusBar()->showMessage("New Window...", 3000);
}

void MainWindow::slotLoad()
{
    if(m_pma->activeSubWindow() == nullptr)
    {
        SingleWindow* pSWindow = createNewSingleWindow();
        pSWindow->slotLoad();
        pSWindow->showMaximized();
        statusBar()->showMessage("File Open...", 3000);
        return;
    }
    else
    {
        qobject_cast<SingleWindow*>(m_pma->activeSubWindow()->widget())->slotLoad();
    }
    statusBar()->showMessage("File Open...", 3000);
}

void MainWindow::slotSave()
{
}

void MainWindow::slotSaveAs()
{
}

void MainWindow::slotSaveAll()
{
}

void MainWindow::slotWindows()
{
    m_pmnuWindows->clear();

    QAction* pact = m_pmnuWindows->addAction("&Cascade", m_pma, SLOT(cascadeSubWindows()));
    pact->setEnabled(!m_pma->subWindowList().isEmpty());

    pact = m_pmnuWindows->addAction("&Tile", m_pma, SLOT(tileSubWindows()));
    pact->setEnabled(!m_pma->subWindowList().isEmpty());

    m_pmnuWindows->addSeparator();

    QList<QMdiSubWindow*> listSubWindows = m_pma->subWindowList();
    for (int i = 0; i < listSubWindows.size(); ++i)
    {
        pact = m_pmnuWindows->addAction(listSubWindows.at(i)->windowTitle());
        pact->setCheckable(true);
        pact->setChecked(m_pma->activeSubWindow() == listSubWindows.at(i));
        connect(pact, SIGNAL(triggered()), m_psigMapper, SLOT(map()));
        m_psigMapper->setMapping(pact, listSubWindows.at(i));
    }
}

void MainWindow::slotAbout()
{
}

void MainWindow::slotSetActiveSubWindow (QWidget* pwgt)
{
    if(pwgt)
        m_pma->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(pwgt));
}
