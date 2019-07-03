#include "spectrum_list_model.h"

ctrl::SpectrumListModel::SpectrumListModel(QObject* parent)
    : QAbstractListModel (parent),
      m_orderNumberLoadedSpectrum(0) {
}

const QList<ctrl::SpectrumPenStruct>& ctrl::SpectrumListModel::getSpecList() const {
    return m_specList;
}

void ctrl::SpectrumListModel::addSpectrum(const SpectrumSPM& newSpectrum) {
    if(m_activatedSpectrumIndex.isValid() && !m_specList.isEmpty()){
        SpectrumPenStruct previousActivatedSpectrumPenStruct;
        previousActivatedSpectrumPenStruct = m_specList.at(m_activatedSpectrumIndex.row());
        previousActivatedSpectrumPenStruct.penForChart.setWidth(DEFAULT_WIDTH_OF_LINE_SPECTRUM_ON_THE_CHART);
        previousActivatedSpectrumPenStruct.textColorForView = Qt::black;
        m_specList.replace(m_activatedSpectrumIndex.row(), previousActivatedSpectrumPenStruct);
    }

    SpectrumPenStruct spectrumPenStruct;
    spectrumPenStruct.penForChart = srvcSpec::getPenForSpec(m_orderNumberLoadedSpectrum++, DEFAULT_WIDTH_OF_LINE_SPECTRUM_ON_THE_CHART);
    spectrumPenStruct.textColorForView = Qt::red;
    spectrumPenStruct.penForChart.setWidth(WIDTH_OF_LINE_ACTIVATED_SPECTRUM_ON_THE_CHART);

    spectrumPenStruct.spm = newSpectrum;
    m_specList.push_back(spectrumPenStruct);
    emit dataChanged(QModelIndex(), QModelIndex());
    m_activatedSpectrumIndex = index(m_specList.size() - 1);
    emit updateSpectrums(true);
}

void ctrl::SpectrumListModel::setActivatedSpectrum(const QModelIndex& index){
    if(!index.isValid()) {
        return;
    }
    SpectrumPenStruct previousActivatedSpectrumPenStruct;
    previousActivatedSpectrumPenStruct = m_specList.at(m_activatedSpectrumIndex.row());
    previousActivatedSpectrumPenStruct.penForChart.setWidth(DEFAULT_WIDTH_OF_LINE_SPECTRUM_ON_THE_CHART);
    previousActivatedSpectrumPenStruct.textColorForView = Qt::black;
    m_specList.replace(m_activatedSpectrumIndex.row(), previousActivatedSpectrumPenStruct);

    SpectrumPenStruct activatedSpectrumPenStruct;
    activatedSpectrumPenStruct = m_specList.at(index.row());
    activatedSpectrumPenStruct.penForChart.setWidth(WIDTH_OF_LINE_ACTIVATED_SPECTRUM_ON_THE_CHART);
    activatedSpectrumPenStruct.textColorForView = Qt::red;
    m_specList.replace(index.row(), activatedSpectrumPenStruct);

    emit dataChanged(m_activatedSpectrumIndex, m_activatedSpectrumIndex);
    emit dataChanged(index, index);
    emit updateSpectrums(false);
    m_activatedSpectrumIndex = index;
}

QVariant ctrl::SpectrumListModel::data (const QModelIndex& index, int nRole) const {
    if(!index.isValid()) {
        return QVariant();
    }
    switch (nRole) {
        case (Qt::DisplayRole):
            return QVariant(m_specList.at(index.row()).spm.getSpectrumAttributes().spectrumName);
        case (Qt::EditRole):
            return QVariant(m_specList.at(index.row()).spm.getSpectrumAttributes().spectrumName);
        case (Qt::TextColorRole):
            return QVariant(m_specList.at(index.row()).textColorForView);
        default:
            return QVariant();
    }
}

int ctrl::SpectrumListModel::rowCount(const QModelIndex&) const {
    return m_specList.size();
}

bool ctrl::SpectrumListModel::setData(const QModelIndex& index, const QVariant& value, int nRole) {
    if(!index.isValid()) {
        return false;
    }
    if(nRole == Qt::EditRole){
        SpectrumPenStruct tmpStructPenStruct = m_specList.at(index.row());
        tmpStructPenStruct.spm.setSpectrumName(value.value<QString>());
        m_specList.replace(index.row(), tmpStructPenStruct);
        emit dataChanged(index, index);
        emit updateSpectrums(false);
        return true;
    }
    if(nRole == Qt::TextColorRole){
        SpectrumPenStruct tmpStructPenStruct = m_specList.at(index.row());
        tmpStructPenStruct.textColorForView = value.value<QColor>();
        m_specList.replace(index.row(), tmpStructPenStruct);
        emit dataChanged(index, index);
        emit updateSpectrums(false);
        return true;
    }
    return false;
}

Qt::ItemFlags ctrl::SpectrumListModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractListModel::flags(index);
    return index.isValid() ? (flags | Qt::ItemIsEditable) : flags;
}
