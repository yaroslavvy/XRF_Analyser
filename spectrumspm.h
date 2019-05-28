#ifndef SPECTRUMSPM_H
#define SPECTRUMSPM_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QRegExp>
#include "service.h"

class SpectrumSPM : public QObject
{
    Q_OBJECT
public:
    SpectrumSPM(QObject *parent = nullptr);
    SpectrumSPM(const SpectrumSPM&, QObject *parent = nullptr);

    const SpectrumSPM& operator= (const SpectrumSPM&);

    bool readFromFile(const QString&);
    bool writeToFile(const QString&) const;

    bool setDataSpm (const QVector<double>&);
    const QVector<double>& getDataSpm() const;

    bool setSpectrumName(const QString&);
    const QString& getSpectrumName() const;

    bool setDateTime(const QDateTime&);
    const QDateTime& getDateTime() const;

    bool setStartSpectrum(const double);
    double getStartSpectrum() const;

    bool setFinishSpectrum(const double);
    double getFinishSpectrum() const;

    bool setStep(const double);
    double getStep() const;

    bool setAmperageTube(const double);
    double getAmperageTube() const;

    bool setVoltageTube(const double);
    double getVoltageTube() const;

    bool setExposureSec(const int);
    int getExposureSec() const;

    bool setFilter(const int);
    int getFilter() const;

    bool setT_dead_prc(const double);
    double getT_dead_prc() const;

    bool setT_peak_us(const double);
    double getT_peak_us() const;

    bool setGain(const double);
    double getGain() const;

    bool setTh(const double);
    double getTh() const;

    bool setSp_type(const QString&);
    const QString& getSp_type() const;

    bool setSp_serial(const int);
    int getSp_serial() const;

    bool setT_peak_s_us(const double);
    double getT_peak_s_us() const;

    bool setT_peak_f_ns(const double);
    double getT_peak_f_ns() const;

    bool setTh_fast(const double);
    double getTh_fast() const;

    bool setTh_slow(const double);
    double getTh_slow() const;

    bool setTh_LLD(const double);
    double getTh_LLD() const;

    bool setPUR(const bool);
    bool getPUR() const;

    bool setT_dead_corr_coef(const double);
    double getT_dead_corr_coef() const;

    bool setCnt_slow(const double);
    double getCnt_slow() const;

    bool setCnt_fast(const double);
    double getCnt_fast() const;

    bool setCnt_fast_rst(const double);
    double getCnt_fast_rst() const;

    bool setT_dead_prc_est1(const double);
    double getT_dead_prc_est1() const;

    bool setT_dead_prc_est2(const double);
    double getT_dead_prc_est2() const;

private:
    QString spectrumName = "";
    QDateTime dateTime;
    QVector<double> spm;
    double startSpectrum;
    double finishSpectrum;
    double step;
    double amperageTube;
    double voltageTube = 0.0;
    int exposureSec = 0;
    int filter = 0;
    double T_dead_prc = 0.0;
    double T_peak_us = 0.0;
    double gain = 0.0;
    double th = 0.0;
    QString sp_type = "";
    int sp_serial = 0;
    double T_peak_s_us = 0.0;
    double T_peak_f_ns = 0.0;
    double th_fast = 0.0;
    double th_slow = 0.0;
    double th_LLD = 0.0;
    bool PUR = false;
    double T_dead_corr_coef = 0.0;
    double cnt_slow  = 0.0;
    double cnt_fast = 0.0;
    double cnt_fast_rst = 0.0;
    double T_dead_prc_est1 = 0.0;
    double T_dead_prc_est2 = 0.0;
};

#endif // SPECTRUMSPM_H
