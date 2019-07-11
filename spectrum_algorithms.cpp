#include "spectrum_algorithms.h"
#include "spectrum_spm.h"
#include "gate.h"

double spectrumAlgorithms::findFullIntegralIntensity(const ctrl::SpectrumSPM &spectrum, const ctrl::Gate &gate) {
    if(spectrum.getSpectrumAttributes().intensities.isEmpty()) {
        return 0.0;
    }
    else {
        return 1.0;//TODO: make algorithm implimentation!
    }
}
