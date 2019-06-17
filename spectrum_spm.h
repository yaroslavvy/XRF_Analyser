#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_SPM_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_SPM_H

#include <QVector>
#include <QDateTime>
#include "spectrum_attribute_struct.h"


namespace ctrl {
    class SpectrumSPM {
    public:
        SpectrumSPM();

        void readFromFile(const QString& fileName);
        void writeToFile(const QString& fileName) const;

        const ctrl::SpectrumAttributes& getSpectrumAttributes() const;

        void setIntensities (const QVector<double>& intensities);
        void setSpectrumName(const QString& spectrumName);
        void setDateTime(const QDateTime& dateTime);
        void setEnergyStartSpectrumKev(double start);
        void setEnergyFinishSpectrumKev(double finish);
        void setEnergyStepSpectrumKev(double step);
        void setAmperageTube(double amperage);
        void setVoltageTube(double voltage);
        void setExposureSec(int exposure_s);
        void setPrimaryBeamFilterIndex(int indexFilter);
        void setT_dead_percents(double);
        void setT_peak_us(double);
        void setGain(double);
        void setTh(double);
        void setSpectrometer_type(const QString&);
        void setSpectrometer_serial(int);
        void setT_peak_s_us(double);
        void setT_peak_f_ns(double);
        void setFastChannelLowLevelTreshold(double);
        void setSlowChannelLowLevelTreshold(double);
        void setTh_LLD(double);
        void setPileUpRejectionEnabled(bool);
        void setT_dead_corr_coef(double);
        void setCnt_slow(double);
        void setCnt_fast(double);
        void setCnt_fast_rst(double);
        void setT_dead_percents_est1(double);
        void setT_dead_percents_est2(double);

    private:
        ctrl::SpectrumAttributes m_spectrumAttributes;
    };
}

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_SPM_H
