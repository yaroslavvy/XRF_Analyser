#include "spectrum_list_model.h"

ctrl::SpectrumListModel::SpectrumListModel(QObject* parent)
    : QAbstractListModel (parent),
      m_orderNumberLoadedSpectrum(0) {
}

const QList<ctrl::SpectrumPenStruct>& ctrl::SpectrumListModel::getSpecList() const {
    return m_specList;
}

void ctrl::SpectrumListModel::addSpectrum(const SpectrumSPM& newSpectrum) {
    SpectrumPenStruct spectrumPenStruct;
    spectrumPenStruct.pen = srvcSpec::getPenForSpec(m_orderNumberLoadedSpectrum++);
    spectrumPenStruct.spm = newSpectrum;
    m_specList.push_back(spectrumPenStruct);
    emit dataChanged(QModelIndex(), QModelIndex());
    emit updateSpectrums(true);
}

QVariant ctrl::SpectrumListModel::data (const QModelIndex& index, int nRole) const {
    if(!index.isValid()) {
        return QVariant();
    }
    return (nRole == Qt::DisplayRole || nRole == Qt::EditRole) ? QVariant(m_specList.at(index.row()).spm.getSpectrumAttributes().spectrumName) : QVariant();
}

int ctrl::SpectrumListModel::rowCount(const QModelIndex&) const {
    return m_specList.size();
}

bool ctrl::SpectrumListModel::setData(const QModelIndex& index, const QVariant& value, int nRole) {
    if(index.isValid() && nRole != Qt::EditRole) {
        return false;
    }
    SpectrumPenStruct tmpStructPenStruct = m_specList.at(index.row());
    tmpStructPenStruct.spm.setSpectrumName(value.value<QString>());
    m_specList.replace(index.row(), tmpStructPenStruct);
    emit dataChanged(index, index);
    emit updateSpectrums(false);
    return true;
}

Qt::ItemFlags ctrl::SpectrumListModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractListModel::flags(index);
    return index.isValid() ? (flags | Qt::ItemIsEditable) : flags;
}
