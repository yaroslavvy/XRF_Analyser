#include "gates_table_model.h"
#include <QStandardItem>
#include <QItemSelection>
#include <QDebug>
#include "gate.h"
#include "gate_pen.h"
#include "spectrum_algorithms.h"

ctrl::GatesTableModel::GatesTableModel(QObject* parent)
    : QAbstractTableModel (parent),
      m_xMode(ui::AxisXMode::ENERGY_KEV)
{
}

void ctrl::GatesTableModel::addGate(const ctrl::Gate& newGate) {
    ctrl::GatePen gatePen;
    gatePen.penForChart.setColor(Qt::blue);
    gatePen.penForChart.setWidth(DEFAULT_WIDTH_LINE_FOR_PAINTING_GATES);
    gatePen.penForChart.setStyle(Qt::DashLine);
    gatePen.gate = newGate;
    m_gateList.push_back(gatePen);
    emit updateGates(false);
}

void ctrl::GatesTableModel::removeGate(const QModelIndexList& selectedRows) {
    int bias = 0;
    for(auto &index : selectedRows) {
        m_gateList.removeAt(index.row() - bias);
        removeRow(index.row() - bias);
        ++bias;
    }
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
    emit updateGates(false);
}

const QList<ctrl::GatePen>& ctrl::GatesTableModel::getGateList() const {
    return m_gateList;
}

QVariant ctrl::GatesTableModel::data (const QModelIndex& index, int nRole) const {
    if(!index.isValid()) {
        return QVariant();
    }
    double energyStepSpectrum;
    double energyStartSpectrum;
    switch (index.column()) {
        case (GATE_TABLE_COLUMN::GATE_NAME):
            switch (nRole) {
                case (Qt::DisplayRole):
                    return QVariant(m_gateList.at(index.row()).gate.getGateName());
                case (Qt::EditRole):
                    return QVariant(m_gateList.at(index.row()).gate.getGateName());
                case (Qt::TextAlignmentRole):
                    return QVariant(Qt::AlignCenter);
                default:
                    return QVariant();
            }
        case (GATE_TABLE_COLUMN::LOW_THRESHHOLD):
            switch (nRole) {
                case (Qt::DisplayRole):
                    switch (m_xMode) {
                        case (ui::AxisXMode::ENERGY_KEV):
                            return QVariant(m_gateList.at(index.row()).gate.getEnergyLowThreshhold());
                        case (ui::AxisXMode::CHANNELS):
                            energyStepSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStepSpectrum_kev;
                            if (energyStepSpectrum <= 0.0){
                                return QVariant(0);
                            }
                            energyStartSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStartSpectrum_kev;
                            return QVariant(spectrumAlgorithms::convertEnergyKeVToChannel(energyStartSpectrum, m_gateList.at(index.row()).gate.getEnergyLowThreshhold(), energyStepSpectrum, 0.0));
                        case (ui::AxisXMode::WAVE_LENGTH_NM):
                            return QVariant(ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_NM / m_gateList.at(index.row()).gate.getEnergyHighThreshhold());
                        case (ui::AxisXMode::WAVE_LENGTH_A):
                            return QVariant(ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_A / m_gateList.at(index.row()).gate.getEnergyHighThreshhold());
                    }
                case (Qt::EditRole):
                    switch (m_xMode) {
                        case (ui::AxisXMode::ENERGY_KEV):
                            return QVariant(QString::number(m_gateList.at(index.row()).gate.getEnergyLowThreshhold()));
                        case (ui::AxisXMode::CHANNELS):
                            energyStepSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStepSpectrum_kev;
                            if (energyStepSpectrum <= 0.0){
                                return QVariant(0);
                            }
                            energyStartSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStartSpectrum_kev;
                            return QVariant(QString::number(spectrumAlgorithms::convertEnergyKeVToChannel(energyStartSpectrum, m_gateList.at(index.row()).gate.getEnergyLowThreshhold(), energyStepSpectrum, 0.0)));
                        case (ui::AxisXMode::WAVE_LENGTH_NM):
                            return QVariant(QString::number(ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_NM / m_gateList.at(index.row()).gate.getEnergyHighThreshhold()));
                        case (ui::AxisXMode::WAVE_LENGTH_A):
                            return QVariant(QString::number(ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_A / m_gateList.at(index.row()).gate.getEnergyHighThreshhold()));
                    }
                case (Qt::TextAlignmentRole):
                    return QVariant(Qt::AlignCenter);
                default:
                    return QVariant();
            }
        case (GATE_TABLE_COLUMN::HIGH_THRESHHOLD):
            switch (nRole) {
                case (Qt::DisplayRole):
                    switch (m_xMode) {
                        case (ui::AxisXMode::ENERGY_KEV):
                            return QVariant(m_gateList.at(index.row()).gate.getEnergyHighThreshhold());
                        case (ui::AxisXMode::CHANNELS):
                            energyStepSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStepSpectrum_kev;
                            if (energyStepSpectrum <= 0.0){
                                return QVariant(0);
                            }
                            energyStartSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStartSpectrum_kev;
                            return QVariant(spectrumAlgorithms::convertEnergyKeVToChannel(energyStartSpectrum, m_gateList.at(index.row()).gate.getEnergyHighThreshhold(), energyStepSpectrum, 0.0));
                        case (ui::AxisXMode::WAVE_LENGTH_NM):
                            return QVariant(ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_NM / m_gateList.at(index.row()).gate.getEnergyLowThreshhold());
                        case (ui::AxisXMode::WAVE_LENGTH_A):
                            return QVariant(ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_A / m_gateList.at(index.row()).gate.getEnergyLowThreshhold());
                    }
                case (Qt::EditRole):
                    switch (m_xMode) {
                        case (ui::AxisXMode::ENERGY_KEV):
                            return QVariant(QString::number(m_gateList.at(index.row()).gate.getEnergyHighThreshhold()));
                        case (ui::AxisXMode::CHANNELS):
                            energyStepSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStepSpectrum_kev;
                            if (energyStepSpectrum <= 0.0){
                                return QVariant(0);
                            }
                            energyStartSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStartSpectrum_kev;
                            return QVariant(QString::number(spectrumAlgorithms::convertEnergyKeVToChannel(energyStartSpectrum, m_gateList.at(index.row()).gate.getEnergyHighThreshhold(), energyStepSpectrum, 0.0)));
                        case (ui::AxisXMode::WAVE_LENGTH_NM):
                            return QVariant(QString::number(ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_NM / m_gateList.at(index.row()).gate.getEnergyLowThreshhold()));
                        case (ui::AxisXMode::WAVE_LENGTH_A):
                            return QVariant(QString::number(ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_A / m_gateList.at(index.row()).gate.getEnergyLowThreshhold()));
                    }
                case (Qt::TextAlignmentRole):
                    return QVariant(Qt::AlignCenter);
                default:
                    return QVariant();
            }
        case (GATE_TABLE_COLUMN::FULL_INTEGRAL_INTENSITY):
            switch (nRole) {
                case (Qt::DisplayRole):
                    return QVariant(spectrumAlgorithms::findFullIntegralIntensity(m_activeSpectrum, m_gateList.at(index.row()).gate));
                case (Qt::TextAlignmentRole):
                    return QVariant(Qt::AlignCenter);
                default:
                    return QVariant();
            }
        default:
            return QVariant();
    }
}

int ctrl::GatesTableModel::rowCount(const QModelIndex&) const {
    return m_gateList.size();
}

int ctrl::GatesTableModel::columnCount(const QModelIndex&) const {
    return 4;
}

bool ctrl::GatesTableModel::setData(const QModelIndex& index, const QVariant& value, int nRole) {
    if(!index.isValid() && (nRole != Qt::EditRole)) {
        return false;
    }
    double doubleValue = value.value<double>();
    if(((nRole == GATE_TABLE_COLUMN::LOW_THRESHHOLD) || (nRole == GATE_TABLE_COLUMN::HIGH_THRESHHOLD)) && (doubleValue < 0)) {
        doubleValue = 0.0;
    }
    GatePen gatePen;
    double energyStepSpectrum;
    double energyStartSpectrum;
    switch (index.column()) {

        case (GATE_TABLE_COLUMN::GATE_NAME):
            gatePen = m_gateList.at(index.row());
            gatePen.gate.setGateName(value.value<QString>());
            m_gateList.replace(index.row(), gatePen);
            emit dataChanged(index, index);
            emit updateGates(false);
            return true;

        case (GATE_TABLE_COLUMN::LOW_THRESHHOLD):
            gatePen = m_gateList.at(index.row());

            if (doubleValue > data(this->index(index.row(), 2)).toDouble()) {
                switch (m_xMode) {
                    case (ui::AxisXMode::ENERGY_KEV):
                        gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyHighThreshhold(), gatePen.gate.getEnergyHighThreshhold());
                        break;

                    case (ui::AxisXMode::CHANNELS):
                        gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyHighThreshhold(), gatePen.gate.getEnergyHighThreshhold());
                        break;

                    case (ui::AxisXMode::WAVE_LENGTH_NM):
                        gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyLowThreshhold(), gatePen.gate.getEnergyLowThreshhold());
                        break;

                    case (ui::AxisXMode::WAVE_LENGTH_A):
                        gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyLowThreshhold(), gatePen.gate.getEnergyLowThreshhold());
                        break;
                }
            }
            else {
                switch (m_xMode) {
                    case (ui::AxisXMode::ENERGY_KEV):
                        gatePen.gate.setEnergyThreshholds(doubleValue, gatePen.gate.getEnergyHighThreshhold());
                        break;

                    case (ui::AxisXMode::CHANNELS):
                        energyStepSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStepSpectrum_kev;
                        if (energyStepSpectrum <= 0.0){
                            break;
                        }
                        energyStartSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStartSpectrum_kev;
                        gatePen.gate.setEnergyThreshholds(spectrumAlgorithms::convertChannelToEnergyKeV(energyStartSpectrum, doubleValue, energyStepSpectrum), gatePen.gate.getEnergyHighThreshhold());
                        break;

                    case (ui::AxisXMode::WAVE_LENGTH_NM):
                        if ((doubleValue <= ui::MAX_POSSIBLE_WAVE_LENGTH_NM) && (doubleValue >= ui::MIN_POSSIBLE_WAVE_LENGTH_NM)) {
                            gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyLowThreshhold(), ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_NM / doubleValue);
                        }
                        else {
                            gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyLowThreshhold(), gatePen.gate.getEnergyHighThreshhold());
                        }
                        break;

                    case (ui::AxisXMode::WAVE_LENGTH_A):
                        if ((doubleValue <= ui::MAX_POSSIBLE_WAVE_LENGTH_A) && (doubleValue >= ui::MIN_POSSIBLE_WAVE_LENGTH_A)) {
                            gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyLowThreshhold(), ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_A / doubleValue);
                        }
                        else {
                            gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyLowThreshhold(), gatePen.gate.getEnergyHighThreshhold());
                        }
                        break;
                }
            }

            m_gateList.replace(index.row(), gatePen);
            emit dataChanged(index, index);
            emit updateGates(false);
            return true;

        case (GATE_TABLE_COLUMN::HIGH_THRESHHOLD):
            gatePen = m_gateList.at(index.row());

            if (doubleValue < data(this->index(index.row(), 1)).toDouble()) {
                switch (m_xMode) {
                    case (ui::AxisXMode::ENERGY_KEV):
                        gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyLowThreshhold(), gatePen.gate.getEnergyLowThreshhold());
                        break;

                    case (ui::AxisXMode::CHANNELS):
                        gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyLowThreshhold(), gatePen.gate.getEnergyLowThreshhold());
                        break;

                    case (ui::AxisXMode::WAVE_LENGTH_NM):
                        gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyHighThreshhold(), gatePen.gate.getEnergyHighThreshhold());
                        break;

                    case (ui::AxisXMode::WAVE_LENGTH_A):
                        gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyHighThreshhold(), gatePen.gate.getEnergyHighThreshhold());
                        break;
                }
            }
            else {
                switch (m_xMode) {
                    case (ui::AxisXMode::ENERGY_KEV):
                        gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyLowThreshhold(), doubleValue);
                        break;

                    case (ui::AxisXMode::CHANNELS):
                        energyStepSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStepSpectrum_kev;
                        if (energyStepSpectrum <= 0.0){
                            break;
                        }
                        energyStartSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStartSpectrum_kev;
                        gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyLowThreshhold(), spectrumAlgorithms::convertChannelToEnergyKeV(energyStartSpectrum, doubleValue, energyStepSpectrum));
                        break;

                    case (ui::AxisXMode::WAVE_LENGTH_NM):
                        if ((doubleValue <= ui::MAX_POSSIBLE_WAVE_LENGTH_NM) && (doubleValue >= ui::MIN_POSSIBLE_WAVE_LENGTH_NM)) {
                            gatePen.gate.setEnergyThreshholds(ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_NM / doubleValue, gatePen.gate.getEnergyHighThreshhold());
                        }
                        else {
                            gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyLowThreshhold(), gatePen.gate.getEnergyHighThreshhold());
                        }
                        break;

                    case (ui::AxisXMode::WAVE_LENGTH_A):
                        if ((doubleValue <= ui::MAX_POSSIBLE_WAVE_LENGTH_A) && (doubleValue >= ui::MIN_POSSIBLE_WAVE_LENGTH_A)) {
                            gatePen.gate.setEnergyThreshholds(ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_A / doubleValue, gatePen.gate.getEnergyHighThreshhold());
                        }
                        else {
                            gatePen.gate.setEnergyThreshholds(gatePen.gate.getEnergyLowThreshhold(), gatePen.gate.getEnergyHighThreshhold());
                        }
                        break;
                }
            }

            m_gateList.replace(index.row(), gatePen);
            qDebug() << "High setData: " << doubleValue;
            emit dataChanged(index, index);
            emit updateGates(false);
            return true;

        default:
            return false;
    }
}

QVariant ctrl::GatesTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole) {
        return QVariant();
    }
    switch (orientation) {
        case (Qt::Horizontal):
            switch (section) {
                case(GATE_TABLE_COLUMN::GATE_NAME):
                    return QVariant("Gate");
                case(GATE_TABLE_COLUMN::LOW_THRESHHOLD):
                    switch (m_xMode) {
                        case(ui::AxisXMode::ENERGY_KEV):
                            return QVariant("Left, KeV");
                        case(ui::AxisXMode::CHANNELS):
                            return QVariant("Left, ch");
                        case(ui::AxisXMode::WAVE_LENGTH_NM):
                            return QVariant("Left, nm");
                        case(ui::AxisXMode::WAVE_LENGTH_A):
                            return QVariant("Left, A");
                    }

                case(GATE_TABLE_COLUMN::HIGH_THRESHHOLD):
                    switch (m_xMode) {
                        case(ui::AxisXMode::ENERGY_KEV):
                            return QVariant("Right, KeV");
                        case(ui::AxisXMode::CHANNELS):
                            return QVariant("Right, ch");
                        case(ui::AxisXMode::WAVE_LENGTH_NM):
                            return QVariant("Right, nm");
                        case(ui::AxisXMode::WAVE_LENGTH_A):
                            return QVariant("Right, A");
                    }

                case(GATE_TABLE_COLUMN::FULL_INTEGRAL_INTENSITY):
                    return QVariant("Intensity");
                default:
                    return QVariant();
            }
        case (Qt::Vertical):
            return QVariant(section + 1);
    }
    return QVariant();
}

Qt::ItemFlags ctrl::GatesTableModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
        return (index.isValid())&&(index.column() != GATE_TABLE_COLUMN::FULL_INTEGRAL_INTENSITY) ? (flags | Qt::ItemIsEditable) : flags;
}

void ctrl::GatesTableModel::setActivatedSpectrum(const ctrl::SpectrumSPM spectrum) {
    m_activeSpectrum = spectrum;
    emit dataChanged(QModelIndex(), QModelIndex());
}

void ctrl::GatesTableModel::setSelectedGateThreshholds(double gateLowThreshhold, double gateHighThreshhold, int selectedRow) {
    GatePen gatePen;
    gatePen = m_gateList.at(selectedRow);
    double energyStepSpectrum;
    double energyStartSpectrum;
    switch (m_xMode) {
        case (ui::AxisXMode::ENERGY_KEV):
            gatePen.gate.setEnergyThreshholds(gateLowThreshhold, gateHighThreshhold);
            break;
        case (ui::AxisXMode::CHANNELS):
            energyStepSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStepSpectrum_kev;
            if (energyStepSpectrum <= 0.0){
                break;
            }
            energyStartSpectrum = m_activeSpectrum.getSpectrumAttributes().energyStartSpectrum_kev;
            gatePen.gate.setEnergyThreshholds(spectrumAlgorithms::convertChannelToEnergyKeV(energyStartSpectrum, gateLowThreshhold, energyStepSpectrum), spectrumAlgorithms::convertChannelToEnergyKeV(energyStartSpectrum, gateHighThreshhold, energyStepSpectrum));
            break;
        case (ui::AxisXMode::WAVE_LENGTH_NM):
            gatePen.gate.setEnergyThreshholds(ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_NM / gateHighThreshhold, ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_NM / gateLowThreshhold);
            break;
        case (ui::AxisXMode::WAVE_LENGTH_A):
            gatePen.gate.setEnergyThreshholds(ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_A / gateHighThreshhold, ui::COEF_CONVERT_ENERGY_KEV_TO_WAVE_LENGTH_A / gateLowThreshhold);
            break;
    }
    m_gateList.replace(selectedRow, gatePen);
    emit dataChanged(QModelIndex(), QModelIndex());
}

void ctrl::GatesTableModel::setAxisXMode(ui::AxisXMode mode) {
    m_xMode = mode;

    emit dataChanged(QModelIndex(), QModelIndex());
}
