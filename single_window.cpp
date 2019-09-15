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
#include <QStringLiteral>
#include <QItemSelectionModel>
#include <QGroupBox>
#include <QModelIndexList>
#include "work_area_view.h"
#include "exception.h"
#include "spectrum_list_model.h"
#include "spectrum_list_view.h"
#include "gates_table_view.h"
#include "gates_table_model.h"
#include "tab_spec_window.h"
#include "spectrum_chart.h"
#include "spectrum_spm.h"
#include "exception.h"
#include "service.h"
#include "main_window.h"



ui::SingleWindow::SingleWindow(QWidget *pwgt)
    : QWidget(pwgt),
      serialNumberTab(1) {
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
    connect(m_cBoxModeX, SIGNAL(activated(int)), this, SLOT(slotUpdateViews()));
    connect(m_tab, SIGNAL(modeXChanged(int)), m_cBoxModeX, SLOT(setCurrentIndex(int)));

    m_cBoxModeY = new QComboBox;
    m_cBoxModeY->addItem(tr("Linear scale"));
    m_cBoxModeY->addItem(tr("Log scale"));
    connect(m_cBoxModeY, SIGNAL(activated(int)), m_tab, SLOT(slotSetYModeView(int)));
    connect(m_tab, SIGNAL(modeYChanged(int)), m_cBoxModeY, SLOT(setCurrentIndex(int)));

    m_lstViewSpectrums = new ui::SpectrumListView;

    QLabel* spectrumLabelView = new QLabel(tr("Spectrum List"));
    spectrumLabelView->setAlignment(Qt::AlignCenter);
    QFont spectrumLabelViewFont = spectrumLabelView->font();
    spectrumLabelViewFont.setBold(true);
    spectrumLabelView->setFont(spectrumLabelViewFont);

    m_tblViewGates = new ui::GatesTableView;

    QLabel* gatesLabelView = new QLabel(tr("Energy Gates"));
    gatesLabelView->setAlignment(Qt::AlignCenter);
    QFont gatesLabelViewFont = gatesLabelView->font();
    gatesLabelViewFont.setBold(true);
    gatesLabelView->setFont(gatesLabelViewFont);

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

    QAction* buttonAddGate = new QAction(tr("Add Gate"), nullptr);
    buttonAddGate->setText(tr("Add &Gate"));
    buttonAddGate->setShortcut(QKeySequence("CTRL+G"));
    buttonAddGate->setToolTip(tr("Create a new energy gate at the list"));
    buttonAddGate->setWhatsThis(tr("Create a new energy gate at the list"));
    buttonAddGate->setIcon(QPixmap("resources/pictures/menuIcons/defaultStyle/addTab26.png"));
    connect(buttonAddGate, SIGNAL(triggered()), this, SLOT(slotAddGate()));

    QToolBar* ptbGate = new QToolBar(tr("Add Gate"));
    ptbGate->addAction(buttonAddGate);
    ptbGate->setOrientation(Qt::Horizontal);

    QHBoxLayout* hbLayoutSubGatesForAddButtonAndLabel = new QHBoxLayout;
    hbLayoutSubGatesForAddButtonAndLabel->addStretch(1);
    hbLayoutSubGatesForAddButtonAndLabel->addWidget(ptbGate);
    hbLayoutSubGatesForAddButtonAndLabel->addStretch(3);
    hbLayoutSubGatesForAddButtonAndLabel->addWidget(gatesLabelView);
    hbLayoutSubGatesForAddButtonAndLabel->addStretch(5);
    hbLayoutSubGatesForAddButtonAndLabel->setMargin(0);

    QVBoxLayout* vbLayoutGatesListView = new QVBoxLayout;
    vbLayoutGatesListView->addLayout(hbLayoutSubGatesForAddButtonAndLabel);
    vbLayoutGatesListView->addWidget(m_tblViewGates);
    vbLayoutGatesListView->setMargin(6);

    QGroupBox* gatesListGroupBox = new QGroupBox;
    gatesListGroupBox->setLayout(vbLayoutGatesListView);

    vSpl2->addWidget(spectrumListGroupBox);
    vSpl2->addWidget(gatesListGroupBox);

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
    const QStringList fileList = QFileDialog::getOpenFileNames(nullptr, tr("Open File"), "resources/spectrums/", "*.* ;; *.spm ;; *.csv");
    if(fileList.isEmpty()) {
        return;
    }

    for (auto &fileName : fileList) {
        try {
            if (!QFile::exists(fileName)) {
                throw ctrl::Exception(fileName + QTranslator::tr(" File not found\n"));
            }
            QFile file(fileName);
            QFileInfo fileInfo(file);
            if(!file.open(QIODevice::ReadOnly)) {
                throw ctrl::Exception(fileInfo.fileName() + QTranslator::tr(" Opening is failed\n"));
            }

            QStringList strFileNameLst = fileName.split('.');
            QString fileExtension = strFileNameLst.last();

            if (fileExtension == "spm") {
                ctrl::SpectrumSPM spm;

                spm.readFromFile(file);

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

            if (fileExtension == "csv") {
                QTextStream in(&file);
                in.setCodec("UTF-8");
                while (!in.atEnd()) {
                    ctrl::Gate gate;
                    try {
                        gate.readFromString(in.readLine());
                    }
                    catch (const ctrl::Exception &ex) {
                        throw ctrl::Exception(fileInfo.fileName() + ex.what());
                    }
                    if(m_tab->count() == 0) {
                        slotAddTab();
                    }
                    m_tab->getCurrentWorkAreaView()->getSpectrumChart()->getModelGates()->addGate(gate);
                }
            }

        }
        catch (const ctrl::Exception &ex){
            QMessageBox::critical(nullptr, tr("Error!"), tr("Error of the file reading! ") + QString(ex.what()), QMessageBox::Cancel);
            return;
        }
        catch (const std::invalid_argument& ia){
            QMessageBox::critical(nullptr, tr("Error!"), tr("Error of the file reading! Invlid argument: ") + QString(ia.what()), QMessageBox::Cancel);
            return;
        }
    }
    slotUpdateViews();
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

    m_lstViewSpectrums->setModel(chart->getModelSpectrums());
    m_lstViewSpectrums->setSelectionModel(chart->getSelectionModelSpectrums());

    m_tblViewGates->setModel(nullptr);
    m_tblViewGates->setModel(chart->getModelGates());
    m_tblViewGates->setSelectionModel(chart->getSelectionModelGates());
    m_tblViewGates->setRowHeight(0, 10);

    view->setChart(chart);
    QString nameForNewTab(tr("Tab № "));
    m_tab->addTab(view, nameForNewTab + QString::number(serialNumberTab++));
    m_tab->setCurrentIndex(m_tab->count() - 1);
    ui::MainWindow* mainWindow = ui::MainWindow::getInstance();
    if (mainWindow != nullptr) {
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SELECT_ALL_ITEMS, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DESELECT_ALL_ITEMS, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::INVERT_SELECTION, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::SHOW_HIDE_ITEMS, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_PRESENTATION_SETTINGS, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::ITEM_INFORMATION, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::DELETE_ITEMS, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::COPY_ITEMS, false);
        mainWindow->setButtonEnable(MAIN_WINDOW_BUTTONS::PASTE_ITEMS, false);
    }
}

void ui::SingleWindow::slotRemoveTab() {
    if(m_tab->count() == 1) {
        QMessageBox::information(nullptr, tr("Information"), tr("Last Tab can not be closed. If you want to close this tab just close this subwindow."), QMessageBox::Ok);
    }
    else {
        m_tab->removeTab(m_tab->currentIndex());
    }
}

void ui::SingleWindow::slotAddGate() {
    ctrl::Gate gate;
    if(m_tab->count() == 0) {
        slotAddTab();
    }
    m_tab->getCurrentWorkAreaView()->getSpectrumChart()->getModelGates()->addGate(gate);
    slotUpdateViews();
}

void ui::SingleWindow::slotUpdateViews(){
    ui::SpectrumChart* chart(m_tab->getCurrentWorkAreaView()->getSpectrumChart());

    m_lstViewSpectrums->setModel(chart->getModelSpectrums());
    m_lstViewSpectrums->setSelectionModel(chart->getSelectionModelSpectrums());

    m_tblViewGates->setModel(nullptr);
    m_tblViewGates->setModel(chart->getModelGates());
    m_tblViewGates->setSelectionModel(chart->getSelectionModelGates());
}
