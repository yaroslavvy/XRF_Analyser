#include "gates_table_model.h"
#include "QStandardItem"
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
    QList<int> uniqueRowsList;
    for (auto &index : indexList) {
        if(!index.isValid()) {
            return;
        }
        uniqueRowsList.push_back(index.row());
    }
    std::sort(uniqueRowsList.begin(), uniqueRowsList.end());
    QList<int>::const_iterator endIterUniqueRowsList = std::unique(uniqueRowsList.begin(), uniqueRowsList.end());
    int bias = 0;
    for(QList<int>::const_iterator i = uniqueRowsList.begin(); i != endIterUniqueRowsList; ++i) {
        m_gateList.removeAt(*i - bias);
        removeRow(*i - bias);
        ++bias;
    }
    emit beginInsertRows(QModelIndex(), rowCount(), rowCount());
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
                case (Qt::TextAlignmentRole):
                    return QVariant(Qt::AlignCenter);
                default:
                    return QVariant();
            }
        case (GATE_TABLE_COLUMN::LOW_THRESHHOLD):
            switch (nRole) {
                case (Qt::DisplayRole):
                    return QVariant(m_gateList.at(index.row()).gate.getEnergyLowThreshhold());
                case (Qt::EditRole):
                    return QVariant(m_gateList.at(index.row()).gate.getEnergyLowThreshhold());
                case (Qt::TextAlignmentRole):
                    return QVariant(Qt::AlignCenter);
                default:
                    return QVariant();
            }
        case (GATE_TABLE_COLUMN::HIGH_THRESHHOLD):
            switch (nRole) {
                case (Qt::DisplayRole):
                    return QVariant(m_gateList.at(index.row()).gate.getEnergyHighThreshhold());
                case (Qt::EditRole):
                    return QVariant(m_gateList.at(index.row()).gate.getEnergyHighThreshhold());
                case (Qt::TextAlignmentRole):
                    return QVariant(Qt::AlignCenter);
                default:
                    return QVariant();
            }
        case (GATE_TABLE_COLUMN::FULL_INTEGRAL_INTENSITY):
            switch (nRole) {
                case (Qt::DisplayRole):
                    return QVariant(spectrumAlgorithms::findFullIntegralIntensity(ctrl::SpectrumSPM()/*TODO: load activated spectrum*/, m_gateList.at(index.row()).gate));
                case (Qt::EditRole):
                    return QVariant(spectrumAlgorithms::findFullIntegralIntensity(ctrl::SpectrumSPM()/*TODO: load activated spectrum*/, m_gateList.at(index.row()).gate));
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
        doubleValue = 0;
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

            if(doubleValue > data(this->index(index.row(), 2)).toDouble()) {
                gatePen.gate.setEnergyLowThreshhold(data(this->index(index.row(), 2)).toDouble());
            }
            else {
                gatePen.gate.setEnergyLowThreshhold(doubleValue);
            }

            m_gateList.replace(index.row(), gatePen);
            emit dataChanged(index, index);
            emit updateGates(false);
            return true;
        case (GATE_TABLE_COLUMN::HIGH_THRESHHOLD):
            gatePen = m_gateList.at(index.row());

            if(doubleValue < data(this->index(index.row(), 1)).toDouble()) {
                gatePen.gate.setEnergyHighThreshhold(data(this->index(index.row(), 1)).toDouble());
            }
            else {
                gatePen.gate.setEnergyHighThreshhold(doubleValue);
            }

            m_gateList.replace(index.row(), gatePen);
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
                    return QVariant("Gate name");
                case(GATE_TABLE_COLUMN::LOW_THRESHHOLD):
                    return QVariant("Left limit");
                case(GATE_TABLE_COLUMN::HIGH_THRESHHOLD):
                    return QVariant("Right limit");
                case(GATE_TABLE_COLUMN::FULL_INTEGRAL_INTENSITY):
                    return QVariant("Intensity");
                default:
                    return QVariant();
            }
        case (Qt::Vertical):
            return QVariant(section + 1);
    }
}

Qt::ItemFlags ctrl::GatesTableModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
        return (index.isValid())&&(index.column() != GATE_TABLE_COLUMN::FULL_INTEGRAL_INTENSITY) ? (flags | Qt::ItemIsEditable) : flags;
}
