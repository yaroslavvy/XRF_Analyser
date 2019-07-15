#ifndef QUALITATIVE_ANALYSIS_GATES_TABLE_MODEL_H
#define QUALITATIVE_ANALYSIS_GATES_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include "gate.h"
#include "gate_pen.h"
#include "spectrum_spm.h"

class QItemSelection;

namespace ctrl {

    enum GATE_TABLE_COLUMN {
        GATE_NAME,
        LOW_THRESHHOLD,
        HIGH_THRESHHOLD,
        FULL_INTEGRAL_INTENSITY
    };

    class GatesTableModel : public QAbstractTableModel {

        Q_OBJECT

    public:
        GatesTableModel(QObject* parent = nullptr);

        void addGate(const ctrl::Gate& newGate);
        void removeGate(const QModelIndexList& selectedRows);
        const QList<ctrl::GatePen>& getGateList() const;

        QVariant data (const QModelIndex& index, int nRole = Qt::DisplayRole) const override;
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int nRole) override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        void setActivatedSpectrum(const ctrl::SpectrumSPM);
        void setSelectedGateThreshholds(double gateLowThreshhold, double gateHighThreshhold, int selectedRow);

    signals:
        void updateGates(bool resizeAxis);

    private:
        QList<GatePen> m_gateList;

        ctrl::SpectrumSPM m_activeSpectrum;

        const int DEFAULT_WIDTH_LINE_FOR_PAINTING_GATES = 1;
    };
}

#endif // QUALITATIVE_ANALYSIS_GATES_TABLE_MODEL_H
