#include "single_window.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QSplitter>
#include <QListView>
#include <QToolBar>
#include <QAction>
#include <QLineSeries>
#include <QValueAxis>
#include <QAbstractAxis>
#include <QTranslator>
#include <QComboBox>
#include <stdexcept>
#include <QDebug>
#include <QItemSelectionModel>
#include <QGroupBox>
#include "work_area_view.h"
#include "spectrum_list_model.h"
#include "spectrum_chart.h"
#include "spectrum_spm.h"
#include "exception.h"
#include "service.h"

ui::SingleWindow::SingleWindow(QWidget *pwgt)
    : QWidget(pwgt) {
    QSplitter* hSpl = new QSplitter(Qt::Horizontal);
    QSplitter* vSpl1 = new QSplitter(Qt::Vertical);
    QSplitter* vSpl2 = new QSplitter(Qt::Vertical);

    m_tab = new TabSpecWindow;
    connect(m_tab, SIGNAL(modelsChartChanged()), this, SLOT(slotUpdateViews()));

    m_cBoxModeX = new QComboBox;
    m_cBoxModeX->addItem(tr("Energy, KeV"));
    m_cBoxModeX->addItem(tr("Channels"));
    m_cBoxModeX->addItem(tr("Wave length, nm"));
    m_cBoxModeX->addItem(tr("Wave length, A"));
    connect(m_cBoxModeX, SIGNAL(activated(int)), m_tab, SLOT(slotSetXModeView(int)));
    connect(m_tab, SIGNAL(modeXChanged(int)), m_cBoxModeX, SLOT(setCurrentIndex(int)));

    m_cBoxModeY = new QComboBox;
    m_cBoxModeY->addItem(tr("Linear scale"));
    m_cBoxModeY->addItem(tr("Log scale"));
    connect(m_cBoxModeY, SIGNAL(activated(int)), m_tab, SLOT(slotSetYModeView(int)));
    connect(m_tab, SIGNAL(modeYChanged(int)), m_cBoxModeY, SLOT(setCurrentIndex(int)));

    m_lstViewSpectrums = new ui::SpectrumListView;
    m_lstViewSpectrums->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_lstViewSpectrums->setEditTriggers(QAbstractItemView::SelectedClicked);

    QLabel* spectrumLabelView = new QLabel(tr("Spectrum List"));
    spectrumLabelView->setAlignment(Qt::AlignCenter);
    QFont spectrumLabelViewFont = spectrumLabelView->font();
    spectrumLabelViewFont.setBold(true);
    spectrumLabelView->setFont(spectrumLabelViewFont);

    QLabel* gatesLabelView = new QLabel(tr("Energy Gates"));
    gatesLabelView->setAlignment(Qt::AlignCenter);
    QFont gatesLabelViewFont = gatesLabelView->font();
    gatesLabelViewFont.setBold(true);
    gatesLabelView->setFont(gatesLabelViewFont);

    m_lstViewGates = new QListView;

    slotAddTab();

    QAction* buttonAddTab = new QAction(tr("Add Tab"), nullptr);
    buttonAddTab->setText(tr("Add &Tab"));
    buttonAddTab->setShortcut(QKeySequence("CTRL+T"));
    buttonAddTab->setToolTip(tr("Create a new tab at the window"));
    buttonAddTab->setWhatsThis(tr("Create a new tab at the window"));
    buttonAddTab->setIcon(QPixmap("resources/pictures/menuIcons/defaultStyle/addTab26.png"));
    connect(buttonAddTab, SIGNAL(triggered()), this, SLOT(slotAddTab()));

    QAction* buttonRemoveTab = new QAction(tr("Remove Tab"), nullptr);
    buttonRemoveTab->setText(tr("&Remove Tab"));
    buttonRemoveTab->setShortcut(QKeySequence("CTRL+R"));
    buttonRemoveTab->setToolTip(tr("Remove active tab from the window"));
    buttonRemoveTab->setWhatsThis(tr("Remove active tab from the window"));
    buttonRemoveTab->setIcon(QPixmap("resources/pictures/menuIcons/defaultStyle/removeTab26.png"));
    connect(buttonRemoveTab, SIGNAL(triggered()), this, SLOT(slotRemoveTab()));

    QToolBar* ptbTab = new QToolBar(tr("File Operations"));
    ptbTab->addAction(buttonAddTab);
    ptbTab->addAction(buttonRemoveTab);
    ptbTab->setOrientation(Qt::Vertical);

    m_messageWindow = new QTextEdit;
    m_messageWindow->setReadOnly(true);
    m_messageWindow->setTextColor(Qt::green);
    m_messageWindow->append(tr("Window opened..."));
    m_messageWindow->setTextColor(Qt::blue);

    QHBoxLayout* hbLayoutAxisModes = new QHBoxLayout;
    hbLayoutAxisModes->addWidget(m_cBoxModeX);
    hbLayoutAxisModes->addWidget(m_cBoxModeY);
    hbLayoutAxisModes->setMargin(0);

    QVBoxLayout* vbLayoutButtonsTab = new QVBoxLayout;
    vbLayoutButtonsTab->addWidget(m_tab);
    vbLayoutButtonsTab->addLayout(hbLayoutAxisModes);
    vbLayoutButtonsTab->setMargin(0);

    QHBoxLayout* hbLayoutButtonsTab = new QHBoxLayout;
    hbLayoutButtonsTab->addWidget(ptbTab);
    hbLayoutButtonsTab->addLayout(vbLayoutButtonsTab);
    hbLayoutButtonsTab->setMargin(0);

    QWidget* wgtButtonTab = new QWidget;
    wgtButtonTab->setLayout(hbLayoutButtonsTab);

    vSpl1->addWidget(wgtButtonTab);
    vSpl1->addWidget(m_messageWindow);

    QVBoxLayout* vbLayoutSpectrumListView = new QVBoxLayout;
    vbLayoutSpectrumListView->addWidget(spectrumLabelView);
    vbLayoutSpectrumListView->addWidget(m_lstViewSpectrums);
    vbLayoutSpectrumListView->setMargin(6);

    QGroupBox* spectrumListGroupBox = new QGroupBox;
    spectrumListGroupBox->setLayout(vbLayoutSpectrumListView);

    QVBoxLayout* vbLayoutSpectrumListGatesView = new QVBoxLayout;
    vbLayoutSpectrumListGatesView->addWidget(spectrumListGroupBox);

    QVBoxLayout* vbLayoutGatesListView = new QVBoxLayout;
    vbLayoutGatesListView->addWidget(gatesLabelView);
    vbLayoutGatesListView->addWidget(m_lstViewGates);
    vbLayoutGatesListView->setMargin(6);

    QGroupBox* gatesListGroupBox = new QGroupBox;
    gatesListGroupBox->setLayout(vbLayoutGatesListView);

    vbLayoutSpectrumListGatesView->addWidget(gatesListGroupBox);

    vbLayoutSpectrumListGatesView->setMargin(0);
    vSpl2->setLayout(vbLayoutSpectrumListGatesView);

    hSpl->addWidget(vSpl1);
    hSpl->addWidget(vSpl2);
    QHBoxLayout* hbLayout = new QHBoxLayout;
    hbLayout->addWidget(hSpl);
    hbLayout->setMargin(0);
    setLayout(hbLayout);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Unknown spectrum"));
}

void ui::SingleWindow::slotLoad()
{
    const QStringList fileList = QFileDialog::getOpenFileNames(nullptr, tr("Open File"), "resources/spectrums/", "*.spm");
    if(fileList.isEmpty()) {
        return;
    }

    for (auto &name : fileList){
        ctrl::SpectrumSPM spm;

        try {
            spm.readFromFile(name);
        }
        catch (const ctrl::Exception &ex){
            QMessageBox::critical(nullptr, tr("Error!"), tr("Error of the file reading! ") + QString(ex.what()), QMessageBox::Cancel);
            return;
        }
        catch (const std::invalid_argument& ia){
            QMessageBox::critical(nullptr, tr("Error!"), tr("Error of the file reading! Invlid argument: ") + QString(ia.what()), QMessageBox::Cancel);
            return;
        }

        setWindowTitle(spm.getSpectrumAttributes().spectrumName);

        QString text;

        m_messageWindow->setTextColor(Qt::green);
        m_messageWindow->append(tr("File loaded..."));
        m_messageWindow->setTextColor(Qt::blue);
        text += tr("Spectrum name: ") + spm.getSpectrumAttributes().spectrumName + "\n";
        text += tr("Date and time: ") + spm.getSpectrumAttributes().dateTime.toString() + "\n";
        text += tr("Start spectrum: ") + QString::number(spm.getSpectrumAttributes().energyStartSpectrum_kev) + tr(" KeV\n");
        text += tr("Finish spectrum: ") + QString::number(spm.getSpectrumAttributes().energyFinishSpectrum_kev) + tr(" KeV\n");
        text += tr("Step: ") + QString::number(spm.getSpectrumAttributes().energyStepSpectrum_kev) + tr(" KeV\n");
        text += tr("Tube voltage: ") + QString::number(spm.getSpectrumAttributes().voltageTube) + tr(" KeV\n");
        text += tr("Tube amperage: ") + QString::number(spm.getSpectrumAttributes().amperageTube) + tr(" mkA\n");
        text += tr("Tube power: ") + QString::number(spm.getSpectrumAttributes().amperageTube * spm.getSpectrumAttributes().voltageTube * 0.001) + tr(" W\n");
        text += tr("Exposure: ") + QString::number(spm.getSpectrumAttributes().exposure_s) + tr(" s\n");
        text += tr("Filter №") + QString::number(spm.getSpectrumAttributes().primaryBeamFilterIndex) + "\n";
        text += tr("Intensities:\n");
        QVector<double> vecIntensities = spm.getSpectrumAttributes().intensities;
        int channel = 0;

        for (auto &val : vecIntensities) {
            text += QString::number(val);
            if((channel + 1) % 8 == 0) {
                text += "\n";
            }
            else {
                text += " ";
            }
            ++channel;
        }
        m_messageWindow->append(text);

        if(m_tab->count() == 0) {
            slotAddTab();
        }
        m_tab->getCurrentWorkAreaView()->getSpectrumChart()->getModelSpectrums()->addSpectrum(spm);
    }
}

void ui::SingleWindow::slotSave() {
    //TODO: make implementation
}

void ui::SingleWindow::slotSaveAs() {
    //TODO: make implementation
}

void ui::SingleWindow::slotAddTab() {
    WorkAreaView* view = new WorkAreaView;
    SpectrumChart* chart = new SpectrumChart;
    ctrl::SpectrumListModel* specModel = new ctrl::SpectrumListModel(chart);
    chart->setModelSpectrums(specModel);
    m_lstViewSpectrums->setModel(specModel);
    view->setChart(chart);
    m_tab->addTab(view, tr("New Tab"));
    m_tab->setCurrentIndex(m_tab->count() - 1);
}

void ui::SingleWindow::slotRemoveTab() {
    if(m_tab->count() == 1) {
        QMessageBox::information(nullptr, tr("Information"), tr("Last Tab can not be closed. If you want to close this tab just close this subwindow."), QMessageBox::Ok);
    }
    else {
        m_tab->removeTab(m_tab->currentIndex());
    }
}

void ui::SingleWindow::slotUpdateViews(){
    m_lstViewSpectrums->setModel(m_tab->getCurrentWorkAreaView()->getSpectrumChart()->getModelSpectrums());

    //TODO: make same updating with m_lstViewGates
}
