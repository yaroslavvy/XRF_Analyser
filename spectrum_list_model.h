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
        void removeSpectrum(const QModelIndexList& indexList);
        void setActivatedSpectrum(const QModelIndex& index);
        void changeVisibilitySpectrum(const QModelIndex& index);
        QVariant data (const QModelIndex& index, int nRole = Qt::DisplayRole) const override;
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int nRole) override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        const QList<SpectrumPenStruct>& getSpecList() const;
        double getEnergyStepOfActivatedSpectrum() const;
        double getEnergyStartofActivatedSpectrum() const;

    signals:
        void updateSpectrums(bool resizeAxis);
    private:
        QList<SpectrumPenStruct> m_specList;
        int m_orderNumberLoadedSpectrum;
        QModelIndex m_activatedSpectrumIndex;

        const int DEFAULT_WIDTH_OF_LINE_SPECTRUM_ON_THE_CHART = 1;
        const int WIDTH_OF_LINE_ACTIVATED_SPECTRUM_ON_THE_CHART = 2;
    };
}

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_LIST_MODEL_H
