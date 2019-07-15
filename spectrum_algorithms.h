#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_ALGORITHMS_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_ALGORITHMS_H

#include <QVector>

namespace ctrl {
    class SpectrumSPM;
    class Gate;
};

namespace spectrumAlgorithms {
    double findFullIntegralIntensity(const ctrl::SpectrumSPM &spectrum, const ctrl::Gate &gate);
    double findFullIntegralIntensity2(const ctrl::SpectrumSPM &spectrum, const ctrl::Gate &gate);
    double convertChannelToEnergyKeV(double energyStartSpectrum, int channel, double energyStep);
    int convertEnergyKeVToChannel(double energyStartSpectrum, double energyForConvert, double energyStep);
    double findAreaUnderLineFunctionFromLowToHighLimits(double topLeftX, double topLeftY, double topRightX, double topRightY, double lowLimit, double highLimit);
    double findfindAreaUnderPolyLine(QVector<QPair<double, double>> &pointsOfPolyLine); //QVector<QPair<double x, double y>>
    double findEnergyFromChannelMidPoint(double energyStartSpectrum, int channel, double energyStep);

};

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_ALGORITHMS_H
