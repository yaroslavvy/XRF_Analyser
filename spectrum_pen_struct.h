#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_PEN_STRUCT_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_PEN_STRUCT_H

#include "spectrum_spm.h"
#include <QPen>

namespace ctrl {
    struct SpectrumPenStruct
    {
        SpectrumSPM spm;
        QPen pen;
    };
}

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_PEN_STRUCT_H
