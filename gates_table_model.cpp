#include "gates_table_model.h"
#include "gate.h"
#include "gate_pen.h"
#include "spectrum_spm.h"
#include "spectrum_algorithms.h"

ctrl::GatesTableModel::GatesTableModel(QObject* parent)
    : QAbstractTableModel (parent)
{
}

void ctrl::GatesTableModel::addGate(const ctrl::Gate& newGate) {
    ctrl::GatePen gatePen;
    gatePen.penForChart.setColor(Qt::blue);
    gatePen.penForChart.setWidth(DEFAULT_WIDTH_LINE_FOR_PAINTING_GATES);
    gatePen.penForChart.setStyle(Qt::DashLine);
    gatePen.gate = newGate;
    m_gateList.push_back(gatePen);
    emit dataChanged(QModelIndex(), QModelIndex());
    emit updateGates(false);
}

void ctrl::GatesTableModel::removeGate(const QModelIndexList& indexList) {
    int bias = 0;
    for (auto &index : indexList) {
        if(!index.isValid()) {
            return;
        }
        m_gateList.removeAt(index.row() - bias);
        removeRow(index.row() - bias);
        ++bias;
    }
    emit dataChanged(QModelIndex(), QModelIndex());
    emit updateGates(false);
}

const QList<ctrl::GatePen>& ctrl::GatesTableModel::getGateList() const {
    return m_gateList;
}

QVariant ctrl::GatesTableModel::data (const QModelIndex& index, int nRole) const {
    if(!index.isValid()) {
        return QVariant();
    }
    switch (index.column()) {
        case (GATE_TABLE_COLUMN::GATE_NAME):
            switch (nRole) {
                case (Qt::DisplayRole):
                    return QVariant(m_gateList.at(index.row()).gate.getGateName());
                case (Qt::EditRole):
                    return QVariant(m_gateList.at(index.row()).gate.getGateName());
                default:
                    return QVariant();
            }
        case (GATE_TABLE_COLUMN::LOW_THRESHHOLD):
            switch (nRole) {
                case (Qt::DisplayRole):
                    return QVariant(m_gateList.at(index.row()).gate.getEnergyLowThreshhold());
                case (Qt::EditRole):
                    return QVariant(m_gateList.at(index.row()).gate.getEnergyLowThreshhold());
                default:
                    return QVariant();
            }
        case (GATE_TABLE_COLUMN::HIGH_THRESHHOLD):
            switch (nRole) {
                case (Qt::DisplayRole):
                    return QVariant(m_gateList.at(index.row()).gate.getEnergyHighThreshhold());
                case (Qt::EditRole):
                    return QVariant(m_gateList.at(index.row()).gate.getEnergyHighThreshhold());
                default:
                    return QVariant();
            }
        case (GATE_TABLE_COLUMN::FULL_INTEGRAL_INTENSITY):
            switch (nRole) {
                case (Qt::DisplayRole):
                    return QVariant(spectrumAlgorithms::findFullIntegralIntensity(ctrl::SpectrumSPM()/*TODO: load activated spectrum*/, m_gateList.at(index.row()).gate));
                case (Qt::EditRole):
                    return QVariant(spectrumAlgorithms::findFullIntegralIntensity(ctrl::SpectrumSPM()/*TODO: load activated spectrum*/, m_gateList.at(index.row()).gate));
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
    GatePen gatePen;
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
            gatePen.gate.setEnergyLowThreshhold(value.value<double>());
            m_gateList.replace(index.row(), gatePen);
            emit dataChanged(index, index);
            emit updateGates(false);
            return true;
        case (GATE_TABLE_COLUMN::HIGH_THRESHHOLD):
            gatePen = m_gateList.at(index.row());
            gatePen.gate.setEnergyHighThreshhold(value.value<double>());
            m_gateList.replace(index.row(), gatePen);
            emit dataChanged(index, index);
            emit updateGates(false);
            return true;
        default:
            return false;
    }
}

Qt::ItemFlags ctrl::GatesTableModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
        return index.isValid() ? (flags | Qt::ItemIsEditable) : flags;
}
