#include "spectrum_list_model.h"
#include <QFont>

ctrl::SpectrumListModel::SpectrumListModel(QObject* parent)
    : QAbstractListModel (parent),
      m_orderNumberLoadedSpectrum(0),
      m_activatedSpectrumIndex(QModelIndex())
{
}

const QList<ctrl::SpectrumPenStruct>& ctrl::SpectrumListModel::getSpecList() const {
    return m_specList;
}

void ctrl::SpectrumListModel::addSpectrum(const SpectrumSPM& newSpectrum) {
    if(m_activatedSpectrumIndex.isValid() && !m_specList.isEmpty()){
        SpectrumPenStruct previousActivatedSpectrumPenStruct;
        previousActivatedSpectrumPenStruct = m_specList.at(m_activatedSpectrumIndex.row());
        previousActivatedSpectrumPenStruct.penForChart.setWidth(DEFAULT_WIDTH_OF_LINE_SPECTRUM_ON_THE_CHART);
        previousActivatedSpectrumPenStruct.activated = false;
        previousActivatedSpectrumPenStruct.textFontForView.setBold(false);
        m_specList.replace(m_activatedSpectrumIndex.row(), previousActivatedSpectrumPenStruct);//
    }

    SpectrumPenStruct spectrumPenStruct;
    spectrumPenStruct.penForChart = srvcSpec::getPenForSpec(m_orderNumberLoadedSpectrum++, DEFAULT_WIDTH_OF_LINE_SPECTRUM_ON_THE_CHART);
    spectrumPenStruct.penForChart.setWidth(WIDTH_OF_LINE_ACTIVATED_SPECTRUM_ON_THE_CHART);
    spectrumPenStruct.activated = true;
    spectrumPenStruct.textFontForView.setBold(true);

    spectrumPenStruct.spm = newSpectrum;
    m_specList.push_back(spectrumPenStruct);
    emit dataChanged(QModelIndex(), QModelIndex());
    emit updateSpectrums(true);
    m_activatedSpectrumIndex = index((m_specList.size() - 1), 0);
    emit activatedSpectrum(getActiveSpectrum());
}

void ctrl::SpectrumListModel::removeSpectrum(const QModelIndexList& indexList) {
    int bias = 0;
    for (auto &index : indexList) {
        if(!index.isValid()) {
            return;
        }
        m_specList.removeAt(index.row() - bias);
        removeRow(index.row() - bias);
        ++bias;
    }
    int i = 0;
    m_activatedSpectrumIndex = QModelIndex();
    for(auto &spectrumPenStruct : m_specList){
        if(spectrumPenStruct.activated) {
            m_activatedSpectrumIndex = index(i);
            break;
        }
        ++i;
    }
    emit dataChanged(QModelIndex(), QModelIndex());
    emit updateSpectrums(false);
    emit activatedSpectrum(getActiveSpectrum());
}

void ctrl::SpectrumListModel::setActivatedSpectrum(const QModelIndex& index){
    if(!index.isValid()) {
        return;
    }
    if(m_activatedSpectrumIndex.isValid()) {
        SpectrumPenStruct previousActivatedSpectrumPenStruct;
        previousActivatedSpectrumPenStruct = m_specList.at(m_activatedSpectrumIndex.row());
        previousActivatedSpectrumPenStruct.penForChart.setWidth(DEFAULT_WIDTH_OF_LINE_SPECTRUM_ON_THE_CHART);
        previousActivatedSpectrumPenStruct.textFontForView.setBold(false);
        previousActivatedSpectrumPenStruct.activated = false;
        m_specList.replace(m_activatedSpectrumIndex.row(), previousActivatedSpectrumPenStruct);
    }

    SpectrumPenStruct activatedSpectrumPenStruct;
    activatedSpectrumPenStruct = m_specList.at(index.row());
    activatedSpectrumPenStruct.penForChart.setWidth(WIDTH_OF_LINE_ACTIVATED_SPECTRUM_ON_THE_CHART);
    activatedSpectrumPenStruct.textFontForView.setBold(true);
    activatedSpectrumPenStruct.activated = true;
    m_specList.replace(index.row(), activatedSpectrumPenStruct);

    emit dataChanged(QModelIndex(), QModelIndex());
    emit updateSpectrums(false);
    m_activatedSpectrumIndex = index;
    emit activatedSpectrum(getActiveSpectrum());
}

void ctrl::SpectrumListModel::changeVisibilitySpectrum(const QModelIndex& index) {
    if(!index.isValid()) {
        return;
    }
    if(!m_specList.at(index.row()).visible){
        SpectrumPenStruct shownSpectrumPenStruct;
        shownSpectrumPenStruct = m_specList.at(index.row());
        shownSpectrumPenStruct.textFontForView.setStrikeOut(false);
        shownSpectrumPenStruct.visible = true;
        m_specList.replace(index.row(), shownSpectrumPenStruct);
    }
    else {
        SpectrumPenStruct hiddenSpectrumPenStruct;
        hiddenSpectrumPenStruct = m_specList.at(index.row());
        hiddenSpectrumPenStruct.textFontForView.setStrikeOut(true);
        hiddenSpectrumPenStruct.visible = false;
        m_specList.replace(index.row(), hiddenSpectrumPenStruct);
    }
    emit dataChanged(index, index);
    emit updateSpectrums(false);
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
        case (Qt::FontRole):
            return QVariant(m_specList.at(index.row()).textFontForView);
        case (Qt::DecorationRole):
            return QVariant(m_specList.at(index.row()).penForChart.color());
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
    if(nRole == Qt::FontRole){
        SpectrumPenStruct tmpStructPenStruct = m_specList.at(index.row());
        tmpStructPenStruct.textFontForView = value.value<QFont>();
        m_specList.replace(index.row(), tmpStructPenStruct);
        emit dataChanged(index, index);
        emit updateSpectrums(false);
        return true;
    }
    if(nRole == Qt::DecorationRole){
        SpectrumPenStruct tmpStructPenStruct = m_specList.at(index.row());
        tmpStructPenStruct.penForChart.color() = value.value<QColor>();
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

double ctrl::SpectrumListModel::getEnergyStepOfActivatedSpectrum() const {
    return m_specList.at(m_activatedSpectrumIndex.row()).spm.getSpectrumAttributes().energyStepSpectrum_kev;
}

double ctrl::SpectrumListModel::getEnergyStartofActivatedSpectrum() const {
    return m_specList.at(m_activatedSpectrumIndex.row()).spm.getSpectrumAttributes().energyStartSpectrum_kev;
}

const ctrl::SpectrumSPM ctrl::SpectrumListModel::getActiveSpectrum() const {
    if(m_activatedSpectrumIndex.isValid()) {
        return m_specList.at(m_activatedSpectrumIndex.row()).spm;
    }
    else {
        return SpectrumSPM();
    }

}
