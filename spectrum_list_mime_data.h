#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_LIST_MIME_DATA_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_LIST_MIME_DATA_H

#include <QMimeData>
#include "spectrum_spm.h"

namespace ctrl {
    class SpectrumListMimeData : public QMimeData {
    public:
        SpectrumListMimeData();
        void setSpectrumList(const QList<ctrl::SpectrumSPM> &spectrumList);
        const QList<ctrl::SpectrumSPM> getSpectrumList() const;

    static QString mimeType();

    private:
        QList<ctrl::SpectrumSPM> m_spectrumList;

    };
}

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_LIST_MIME_DATA_H
