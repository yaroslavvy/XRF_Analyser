#include "gate_table_mime_data.h"
#include "gate.h"

ctrl::GateTableMimeData::GateTableMimeData()
    :QMimeData ()
{
}

void ctrl::GateTableMimeData::setGateList(const QList<ctrl::Gate> &gateList) {
    m_gateList = gateList;
    setData(mimeType(), QByteArray());
}

const QList<ctrl::Gate> ctrl::GateTableMimeData::getGateList() const {
    return m_gateList;
}

QString ctrl::GateTableMimeData::mimeType() {
    return "application/gate_list";
}
