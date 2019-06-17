#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_CHART_AXIS_MODES_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_CHART_AXIS_MODES_H

namespace ui {

    enum AxisXMode{
        ENERGY_KEV,
        CHANNELS,
        WAVE_LENGTH_NM,
        WAVE_LENGTH_A
    };

    enum AxisYMode{
        LINEAR,
        LOG
    };
}

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_CHART_AXIS_MODES_H
