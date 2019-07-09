#ifndef QUALITATIVE_ANALYSIS_GATE_TABLE_MIME_DATA_H
#define QUALITATIVE_ANALYSIS_GATE_TABLE_MIME_DATA_H

#include <QMimeData>

namespace ctrl {
    class Gate;

    class GateTableMimeData : public QMimeData {
    public:
        GateTableMimeData();
        void setGateList(const QList<ctrl::Gate> &gateList);
        const QList<ctrl::Gate> getGateList() const;

        static QString mimeType();

    private:
        QList<ctrl::Gate> m_gateList;
    };
}

#endif // QUALITATIVE_ANALYSIS_GATE_TABLE_MIME_DATA_H
