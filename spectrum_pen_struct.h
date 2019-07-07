#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_PEN_STRUCT_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_PEN_STRUCT_H

#include "spectrum_spm.h"
#include <QPen>
#include <QColor>
#include <QFont>
#include <QBrush>


namespace ctrl {
    struct SpectrumPenStruct
    {
        SpectrumSPM spm;
        QPen penForChart;
        QFont textFontForView;
        QBrush backgroundColor;
        bool visible = true;
        bool activated = false;
    };
}

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_PEN_STRUCT_H
