#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_ALGORITHMS_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_ALGORITHMS_H

namespace ctrl {
    class SpectrumSPM;
    class Gate;
};

namespace spectrumAlgorithms {
    double findFullIntegralIntensity(const ctrl::SpectrumSPM &spectrum, const ctrl::Gate &gate);
};

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_ALGORITHMS_H
