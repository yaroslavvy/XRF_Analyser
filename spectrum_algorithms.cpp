#include "spectrum_algorithms.h"
#include "spectrum_spm.h"
#include "gate.h"
#include <QPair>

double spectrumAlgorithms::findFullIntegralIntensity(const ctrl::SpectrumSPM &spectrum, const ctrl::Gate &gate) {
    if(spectrum.getSpectrumAttributes().intensities.isEmpty()) {
        return 0.0;
    }
    double energyStartSpectrum = spectrum.getSpectrumAttributes().energyStartSpectrum_kev;
    double energyStep = spectrum.getSpectrumAttributes().energyStepSpectrum_kev;
    int channelLowThreshhold = spectrumAlgorithms::convertEnergyKeVToChannel(energyStartSpectrum, gate.getEnergyLowThreshhold(), energyStep);
    int channelHighThreshhold = spectrumAlgorithms::convertEnergyKeVToChannel(energyStartSpectrum, gate.getEnergyHighThreshhold(), energyStep);
    QVector<double> intensities = spectrum.getSpectrumAttributes().intensities;
    if (channelHighThreshhold > (intensities.size() - 1)) {
        return 0.0;
    }
    double integralIntensity = 0.0;
    integralIntensity = std::accumulate(intensities.cbegin() + channelLowThreshhold, intensities.cbegin() + channelHighThreshhold + 1, 0.0);
    return integralIntensity;
}

double spectrumAlgorithms::findFullIntegralIntensity2(const ctrl::SpectrumSPM &spectrum, const ctrl::Gate &gate) {
    /*if(spectrum.getSpectrumAttributes().intensities.isEmpty()) {
        return 0.0;
    }

    double gateEnergyLowThreshhold = gate.getEnergyLowThreshhold();
    double energyStartSpectrum = spectrum.getSpectrumAttributes().energyStartSpectrum_kev;
    if(gateEnergyLowThreshhold < energyStartSpectrum) {
        gateEnergyLowThreshhold = energyStartSpectrum;
    }

    double gateEnergyHighThreshhold = gate.getEnergyHighThreshhold();
    double energyFinishSpectrum = spectrum.getSpectrumAttributes().energyFinishSpectrum_kev;
    if(gateEnergyHighThreshhold > energyFinishSpectrum) {
        gateEnergyHighThreshhold = energyFinishSpectrum;
    }

    double energyStep = spectrum.getSpectrumAttributes().energyStepSpectrum_kev;

    int channelLowThreshhold = spectrumAlgorithms::convertEnergyKeVToChannel(energyStartSpectrum, gateEnergyLowThreshhold, energyStep);
    double midpointEnergyChannelLowThreshhold = findEnergyFromChannelMidPoint(energyStartSpectrum, channelLowThreshhold, energyStep);

    int channelHighThreshhold = spectrumAlgorithms::convertEnergyKeVToChannel(energyStartSpectrum, gateEnergyHighThreshhold, energyStep);
    double midpointChannelHighThreshhold = findEnergyFromChannelMidPoint(energyStartSpectrum, channelHighThreshhold, energyStep);

    QVector<double> intensities = spectrum.getSpectrumAttributes().intensities;

    if(gateEnergyLowThreshhold < midpointEnergyChannelLowThreshhold) {
        if (channelLowThreshhold != 0) {
            double midpointEnergyPreviousChannelLowThreshhold = findEnergyFromChannelMidPoint(energyStartSpectrum, channelLowThreshhold - 1, energyStep);
            if()
        }
    }
*/
}

double spectrumAlgorithms::convertChannelToEnergyKeV (double energyStartSpectrum, int channel, double energyStep) {
    return energyStartSpectrum + (channel * energyStep);
}

int spectrumAlgorithms::convertEnergyKeVToChannel (double energyStartSpectrum, double energyForConvert, double energyStep) {
    return static_cast<int>((energyForConvert - energyStartSpectrum) / energyStep);
}



double spectrumAlgorithms::findEnergyFromChannelMidPoint(double energyStartSpectrum, int channel, double energyStep) {
    return energyStartSpectrum + ((channel + 0.5) * energyStep);
}

double spectrumAlgorithms::findAreaUnderLineFunctionFromLowToHighLimits(double topLeftX, double topLeftY, double topRightX, double topRightY, double lowLimit, double highLimit) {
    double slope = (topRightY - topLeftY) / (topRightX - topLeftX);
    double freeMember = topLeftY - slope * topLeftX;
    return (slope * (highLimit * highLimit - lowLimit * lowLimit)) / 2 - freeMember * (highLimit - lowLimit);
}

double spectrumAlgorithms::findfindAreaUnderPolyLine(QVector<QPair<double, double>> &pointsOfPolyLine) { //QVector<QPair<double x, double y>>
    double totalArea = 0.0;
    QPair<double, double> previousPoint (0, -1.0);
    for (auto &point : pointsOfPolyLine) {
        if(point.second < 0) {
            //TODO: throw local exception "All points must be above a X-Axis"
        }
        if(previousPoint.second < 0){
            previousPoint = point;
        }
        else {
            if (point.first < previousPoint.first) {
                //TODO: throw local exception "All points must be sorted by X values"
            }
            totalArea += ((previousPoint.second + point.second) * (point.first - previousPoint.first)) / 2;
            previousPoint = point;
        }
    }
    return totalArea;
}
