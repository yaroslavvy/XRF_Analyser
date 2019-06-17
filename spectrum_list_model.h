#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_LIST_MODEL_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_LIST_MODEL_H

#include <QList>
#include <QAbstractListModel>
#include <QModelIndex>
#include "spectrum_pen_struct.h"
#include "spectrum_spm.h"
#include "service.h"

namespace ctrl {
    class SpectrumListModel : public QAbstractListModel {
        Q_OBJECT
    public:
        SpectrumListModel(QObject* parent = nullptr);
        void addSpectrum(const SpectrumSPM& newSpectrum);
        QVariant data (const QModelIndex& index, int nRole = Qt::DisplayRole) const override;
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int nRole) override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        const QList<SpectrumPenStruct>& getSpecList() const;
    signals:
        void updateSpectrums(bool resizeAxis);
    private:
        QList<SpectrumPenStruct> m_specList;
        int m_orderNumberLoadedSpectrum;
    };
}

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_LIST_MODEL_H
