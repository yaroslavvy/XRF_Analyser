#ifndef QUALITATIVE_ANALYSIS_GATE_PEN_H
#define QUALITATIVE_ANALYSIS_GATE_PEN_H

#include <QPen>
#include <QFont>
#include "gate.h"

namespace ctrl {
    struct GatePen
    {
        ctrl::Gate gate;
        QPen penForChart;
    };
}

#endif // QUALITATIVE_ANALYSIS_GATE_PEN_H
