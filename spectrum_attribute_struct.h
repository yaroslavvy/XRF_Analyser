#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_ATTRIBUTE_STRUCT_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_ATTRIBUTE_STRUCT_H

#include <QString>
#include <QDateTime>
#include <QVector>

namespace ctrl {
    struct SpectrumAttributes {
        QString spectrumName = "";
        QDateTime dateTime;
        QVector<double> intensities;
        double energyStartSpectrum_kev = -1;
        double energyFinishSpectrum_kev = -1;
        double energyStepSpectrum_kev = -1;
        double amperageTube = -1;
        double voltageTube = -1;
        int exposure_s = -1;
        int primaryBeamFilterIndex = -1;
        double T_dead_percents = -1;
        double T_peak_us = -1;
        double gain = -1;
        double th = -1;
        QString spectrometer_type = "";
        int spectrometer_serial = -1;
        double T_peak_s_us = -1;
        double T_peak_f_ns = -1;
        double fastChannelLowLevelTreshold = -1;
        double slowChannelLowLevelTreshold = -1;
        double th_LLD = -1;
        bool pileUpRejectionEnabled = false;
        double T_dead_corr_coef = -1;
        double cnt_slow  = -1;
        double cnt_fast = -1;
        double cnt_fast_rst = -1;
        double T_dead_percents_est1 = -1;
        double T_dead_percents_est2 = -1;
    };
}


#endif // QUALITATIVE_ANALYSIS_SPECTRUM_ATTRIBUTE_STRUCT_H
