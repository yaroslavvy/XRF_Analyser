#include "spectrumlistmodel.h"

SpectrumListModel::SpectrumListModel(QObject* pobj)
    : QAbstractListModel (pobj),
      orderNumberLoadedSpectrum(0)
{
}

const QList<SpectrumPenStruct>& SpectrumListModel::getSpecList()
{
    return specList;
}

void SpectrumListModel::addSpectrum(SpectrumSPM& spec)
{
    SpectrumPenStruct spectrumPenStruct;
    spectrumPenStruct.pen = Service::getPenForSpec(orderNumberLoadedSpectrum++);
    spectrumPenStruct.spm = spec;
    specList.push_back(spectrumPenStruct);
    emit dataChanged(QModelIndex(), QModelIndex());
    emit updateSpectrums(true);
    return;
}

QVariant SpectrumListModel::data (const QModelIndex& index, int nRole) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    return (nRole == Qt::DisplayRole || nRole == Qt::EditRole) ? QVariant(specList.at(index.row()).spm.getSpectrumName()) : QVariant();
}

int SpectrumListModel::rowCount(const QModelIndex& parent) const
{
    return specList.size();
}

bool SpectrumListModel::setData(const QModelIndex& index, const QVariant& value, int nRole)
{
    if(index.isValid() && nRole == Qt::EditRole)
    {
        SpectrumPenStruct tmpStructPenStruct = specList.at(index.row());
        tmpStructPenStruct.spm.setSpectrumName(value.value<QString>());
        specList.replace(index.row(), tmpStructPenStruct);
        emit dataChanged(index, index);
        emit updateSpectrums(false);
        return true;
    }
    return false;
}

Qt::ItemFlags SpectrumListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags(index);
    return index.isValid() ? (flags | Qt::ItemIsEditable) : flags;
}
