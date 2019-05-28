#include "singlewindow.h"

SingleWindow::SingleWindow(QWidget *pwgt)
    : QWidget(pwgt) {
    QSplitter* hSpl = new QSplitter(Qt::Horizontal);
    QSplitter* vSpl1 = new QSplitter(Qt::Vertical);
    QSplitter* vSpl2 = new QSplitter(Qt::Vertical);

    pTab = new TabSpecWindow;
    pTab->setTabShape(TabSpecWindow::Triangular);
    pTab->setDocumentMode(true);
    pTab->setMovable(true);
    pTab->setUsesScrollButtons(true);

    pCBmodeX = new QComboBox;
    pCBmodeX->addItem("Energy, KeV");
    pCBmodeX->addItem("Channels");
    pCBmodeX->addItem("Wave length, nm");
    pCBmodeX->addItem("Wave length, A");
    connect(pCBmodeX, SIGNAL(activated(int)), pTab, SLOT(slotSetXModeView(int)));
    connect(pTab, SIGNAL(modeXChanged(int)), pCBmodeX, SLOT(setCurrentIndex(int)));

    pCBmodeY = new QComboBox;
    pCBmodeY->addItem("Linear scale");
    pCBmodeY->addItem("Log scale");
    connect(pCBmodeY, SIGNAL(activated(int)), pTab, SLOT(slotSetYModeView(int)));
    connect(pTab, SIGNAL(modeYChanged(int)), pCBmodeY, SLOT(setCurrentIndex(int)));

    lstViewSpectrums = new QListView;
    lstViewGates = new QListView;

    slotAddTab();

    QAction* pButtonAddTab = new QAction("Add Tab", nullptr);
    pButtonAddTab->setText("Add &Tab");
    pButtonAddTab->setShortcut(QKeySequence("CTRL+T"));
    pButtonAddTab->setToolTip("Add Tab");
    pButtonAddTab->setToolTip("Create a new tab at the window");
    pButtonAddTab->setWhatsThis("Create a new tab at the window");
    pButtonAddTab->setIcon(QPixmap("resources/pictures/menuIcons/defaultStyle/addTab.png"));
    connect(pButtonAddTab, SIGNAL(triggered()), this, SLOT(slotAddTab()));

    QAction* pButtonRemoveTab = new QAction("Remove Tab", nullptr);
    pButtonRemoveTab->setText("&Remove Tab");
    pButtonRemoveTab->setShortcut(QKeySequence("CTRL+R"));
    pButtonRemoveTab->setToolTip("Remove Tab");
    pButtonRemoveTab->setToolTip("Remove active tab from the window");
    pButtonRemoveTab->setWhatsThis("Remove active tab from the window");
    pButtonRemoveTab->setIcon(QPixmap("resources/pictures/menuIcons/defaultStyle/removeTab.png"));
    connect(pButtonRemoveTab, SIGNAL(triggered()), this, SLOT(slotRemoveTab()));

    QToolBar* ptbTab = new QToolBar("File Operations");
    ptbTab->addAction(pButtonAddTab);
    ptbTab->addAction(pButtonRemoveTab);
    ptbTab->setOrientation(Qt::Vertical);

    messageWindow = new QTextEdit;
    messageWindow->setReadOnly(true);
    messageWindow->setTextColor(Qt::green);
    messageWindow->append("Window opened...");
    messageWindow->setTextColor(Qt::blue);

    QHBoxLayout* phbLayoutAxisModes = new QHBoxLayout;
    phbLayoutAxisModes->addWidget(pCBmodeX);
    phbLayoutAxisModes->addWidget(pCBmodeY);


    QVBoxLayout* pvbLayoutButtonsTab = new QVBoxLayout;
    pvbLayoutButtonsTab->addWidget(pTab);
    pvbLayoutButtonsTab->addLayout(phbLayoutAxisModes);

    QHBoxLayout* phbLayoutButtonsTab = new QHBoxLayout;
    phbLayoutButtonsTab->addWidget(ptbTab);
    phbLayoutButtonsTab->addLayout(pvbLayoutButtonsTab);
    phbLayoutButtonsTab->setMargin(0);

    QWidget* pWgtButtonTab = new QWidget;
    pWgtButtonTab->setLayout(phbLayoutButtonsTab);

    vSpl1->addWidget(pWgtButtonTab);
    vSpl1->addWidget(messageWindow);

    vSpl2->addWidget(lstViewSpectrums);
    vSpl2->addWidget(lstViewGates);

    hSpl->addWidget(vSpl1);
    hSpl->addWidget(vSpl2);
    QHBoxLayout* phbLayout = new QHBoxLayout;
    phbLayout->addWidget(hSpl);
    setLayout(phbLayout);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Unknown spectrum");
}

void SingleWindow::slotLoad()
{    
    QString str = QFileDialog::getOpenFileName(nullptr, "Open File", "resources/spectrums/", "*.spm");
    if(str.isEmpty()) {
        return;
    }

    SpectrumSPM spm;

    if(!spm.readFromFile(str)) {
        QMessageBox::critical(nullptr, "Error!", "Error of the file reading! " + str, QMessageBox::Cancel);
        return;
    }

    setWindowTitle(spm.getSpectrumName());

    QString text;

    messageWindow->setTextColor(Qt::green);
    messageWindow->append("File loaded...");
    messageWindow->setTextColor(Qt::blue);
    text += "Spectrum name: " + spm.getSpectrumName() + "\n";
    text += "Date and time: " + spm.getDateTime().toString() + "\n";
    text += "Start spectrum: " + QString::number(spm.getStartSpectrum()) + " KeV\n";
    text += "Finish spectrum: " + QString::number(spm.getFinishSpectrum()) + " KeV\n";
    text += "Step: " + QString::number(spm.getStep()) + " KeV\n";
    text += "Tube voltage: " + QString::number(spm.getVoltageTube()) + " KeV\n";
    text += "Tube amperage: " + QString::number(spm.getAmperageTube()) + " mkA\n";
    text += "Tube power: " + QString::number(spm.getAmperageTube() * spm.getVoltageTube() * 0.001) + " W\n";
    text += "Exposure: " + QString::number(spm.getExposureSec()) + " s\n";
    text += "Filter №" + QString::number(spm.getFilter()) + "\n";
    text += "Intensities:\n";
    QVector<double> vecIntensities;
    vecIntensities = spm.getDataSpm();
    int channel = 0;

    foreach(double val, vecIntensities) {
        text += QString::number(val);
        (channel + 1) % 8 == 0 ? text += "\n" : text += " ";
        channel++;
    }
    messageWindow->append(text);

    if(pTab->count() == 0) {
        slotAddTab();
    }
    qobject_cast<SpectrumChart*>(qobject_cast<WorkAreaView*>(pTab->currentWidget())->chart())->getModelSpectrums()->addSpectrum(spm);
}

void SingleWindow::slotSave() {
}

void SingleWindow::slotSaveAs() {
}

void SingleWindow::slotAddTab() {
    WorkAreaView* view = new WorkAreaView;
    SpectrumChart* chart = new SpectrumChart;
    SpectrumListModel* specModel = new SpectrumListModel(chart);
    chart->setModelSpectrums(specModel);
    lstViewSpectrums->setModel(specModel);
    view->setChart(chart);
    pTab->addTab(view, "New Tab");
    pTab->setCurrentIndex(pTab->count() - 1);
}

void SingleWindow::slotRemoveTab() {
    if(pTab->count() == 1) {
        QMessageBox::information(nullptr, "Information", "Last Tab can not be closed. If you want to close this tab just close this subwindow.", QMessageBox::Ok);
        return;
    }
    pTab->removeTab(pTab->currentIndex());
}
