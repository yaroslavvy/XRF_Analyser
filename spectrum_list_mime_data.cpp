#include "spectrum_list_mime_data.h"

ctrl::SpectrumListMimeData::SpectrumListMimeData()
    :QMimeData ()
{
}

void ctrl::SpectrumListMimeData::setSpectrumList(const QList<ctrl::SpectrumSPM> &spectrumList){
    m_spectrumList = spectrumList;
    setData(mimeType(), QByteArray());
}

const QList<ctrl::SpectrumSPM> ctrl::SpectrumListMimeData::getSpectrumList() const {
    return m_spectrumList;
}

QString ctrl::SpectrumListMimeData::mimeType(){
    return "application/spectrum_list";
}
