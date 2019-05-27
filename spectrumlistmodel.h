#ifndef SPECTRUMLISTMODEL_H
#define SPECTRUMLISTMODEL_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>
#include <QModelIndex>
#include "spectrumpenstruct.h"
#include "spectrumspm.h"
#include "service.h"

class SpectrumListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    SpectrumListModel(QObject* pobj = nullptr);
    void addSpectrum(SpectrumSPM&);

    QVariant data (const QModelIndex& index, int nRole = Qt::DisplayRole) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    bool setData(const QModelIndex& index, const QVariant& value, int nRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    const QList<SpectrumPenStruct>& getSpecList();

signals:
    void updateSpectrums(bool);

private:
    QList<SpectrumPenStruct> specList;
    int orderNumberLoadedSpectrum;
};

#endif // SPECTRUMLISTMODEL_H
