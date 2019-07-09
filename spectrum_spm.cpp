#include "spectrum_spm.h"
#include "spectrum_spm.h"
#include "exception.h"
#include "service.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QRegExp>
#include <QTranslator>

ctrl::SpectrumSPM::SpectrumSPM() {
}

void ctrl::SpectrumSPM::readFromFile(QFile& file) {
    QFileInfo fileInfo(file);
    m_spectrumAttributes.intensities.clear();
    if(!file.atEnd()) {
        setSpectrumName(fileInfo.fileName());
        QString str = file.readLine();
        QStringList strLst = str.split(' ');
        strLst.removeAll("");
        setEnergyStartSpectrumKev(std::stod(srvcSpec::commaToDot(strLst.takeFirst().toStdString())));
        setEnergyFinishSpectrumKev(std::stod(srvcSpec::commaToDot(strLst.takeFirst().toStdString())));
        setEnergyStepSpectrumKev(std::stod(srvcSpec::commaToDot(strLst.takeFirst().toStdString())));
        setExposureSec(std::stoi(strLst.takeFirst().toStdString()));
        setVoltageTube(std::stod(srvcSpec::commaToDot(strLst.takeFirst().toStdString())));
        setAmperageTube(std::stod(srvcSpec::commaToDot(strLst.takeFirst().toStdString())));
        setPrimaryBeamFilterIndex(std::stoi(strLst.takeFirst().toStdString()));

        strLst.takeFirst(); // TODO: What is value?
        strLst.takeFirst(); // TODO: What is value?
        strLst.takeFirst(); // TODO: What is value?

        QString date(strLst.takeFirst());
        QStringList dateLst(date.split("."));

        int day = std::stoi(dateLst.takeFirst().toStdString());
        int month = std::stoi(dateLst.takeFirst().toStdString());
        int year = std::stoi(dateLst.takeFirst().toStdString());

        QString time(strLst.takeFirst());
        QStringList timeLst(time.split(":"));

        int hour = std::stoi(timeLst.takeFirst().toStdString());
        int minute = std::stoi(timeLst.takeFirst().toStdString());
        int second = std::stoi(timeLst.takeFirst().toStdString());

        QDateTime dT;
        dT.setDate(QDate(year, month, day));
        dT.setTime(QTime(hour, minute, second));
        setDateTime(dT);
        //TODO: make other arguments untill the end
    }
    else {
        throw Exception(fileInfo.fileName() + QTranslator::tr(" Error file reading at the first line\n"));
    }

    if(file.atEnd()) {
        throw Exception(fileInfo.fileName() + QTranslator::tr(" Error file reading after the first line\n"));
    }

    while(!file.atEnd()) {
        QString str = file.readLine();
        QStringList strLst = str.split(' ');
        for(auto &strCell : strLst) {
            if((strCell.isEmpty()) || (strCell.end() != std::find(strCell.begin(), strCell.end(), "\n") )) {
                continue;
            }
            m_spectrumAttributes.intensities.push_back(std::stod(srvcSpec::commaToDot(strCell.toStdString())));
        }
    }
}

void ctrl::SpectrumSPM::writeToFile(const QString& fileName) const {
    (void)fileName;
}

const ctrl::SpectrumAttributes& ctrl::SpectrumSPM::getSpectrumAttributes() const {
    return m_spectrumAttributes;
}
void ctrl::SpectrumSPM::setIntensities (const QVector<double> &a) {
    m_spectrumAttributes.intensities = a;
}

void ctrl::SpectrumSPM::setSpectrumName(const QString &a) {
    m_spectrumAttributes.spectrumName = a;
}

void ctrl::SpectrumSPM::setDateTime(const QDateTime &a) {
    m_spectrumAttributes.dateTime = a;
}

void ctrl::SpectrumSPM::setEnergyStartSpectrumKev(double a) {
    m_spectrumAttributes.energyStartSpectrum_kev = a;
}

void ctrl::SpectrumSPM::setEnergyFinishSpectrumKev(double a) {
    m_spectrumAttributes.energyFinishSpectrum_kev = a;
}

void ctrl::SpectrumSPM::setEnergyStepSpectrumKev(double a) {
    if(a < 0) {
        throw Exception(getSpectrumAttributes().spectrumName + QTranslator::tr(". Step is less than 0 (") + QString::number(a) + ")\n");
    }
    m_spectrumAttributes.energyStepSpectrum_kev = a;
}

void ctrl::SpectrumSPM::setAmperageTube(double a) {
    if(a < 0) {
        throw Exception(getSpectrumAttributes().spectrumName + QTranslator::tr(". Amperage tube is less than 0 (") + QString::number(a) + ")\n");
    }
    m_spectrumAttributes.amperageTube = a;
}

void ctrl::SpectrumSPM::setVoltageTube(double a) {
    if(a < 0) {
        throw Exception(getSpectrumAttributes().spectrumName + QTranslator::tr(". Voltage tube is less than 0 (") + QString::number(a) + ")\n");
    }
    m_spectrumAttributes.voltageTube = a;
}

void ctrl::SpectrumSPM::setExposureSec(int a) {
    if(a < 0) {
        throw Exception(getSpectrumAttributes().spectrumName + QTranslator::tr(". Exposure is less than 0 (") + QString::number(a) + " s)\n");
    }
    m_spectrumAttributes.exposure_s = a;
}

void ctrl::SpectrumSPM::setPrimaryBeamFilterIndex(int a) {
    if(a < 0) {
        throw Exception(getSpectrumAttributes().spectrumName + QTranslator::tr(". Number filter can not be less than 0 (") + QString::number(a) + ")\n");
    }
    m_spectrumAttributes.primaryBeamFilterIndex = a;
}

void ctrl::SpectrumSPM::setT_dead_percents(double a) {
    m_spectrumAttributes.T_dead_percents = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setT_peak_us(double a) {
    m_spectrumAttributes.T_peak_us = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setGain(double a) {
    m_spectrumAttributes.gain = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setTh(double a) {
    m_spectrumAttributes.th = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setSpectrometer_type(const QString &a) {
    m_spectrumAttributes.spectrometer_type = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setSpectrometer_serial(int a) {
    m_spectrumAttributes.spectrometer_serial = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setT_peak_s_us(double a) {
    m_spectrumAttributes.T_peak_s_us = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setT_peak_f_ns(double a) {
    m_spectrumAttributes.T_peak_f_ns = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setFastChannelLowLevelTreshold(double a) {
    m_spectrumAttributes.fastChannelLowLevelTreshold = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setSlowChannelLowLevelTreshold(double a) {
    m_spectrumAttributes.slowChannelLowLevelTreshold = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setTh_LLD(double a) {
    m_spectrumAttributes.th_LLD = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setPileUpRejectionEnabled(bool a) {
    m_spectrumAttributes.pileUpRejectionEnabled = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setT_dead_corr_coef(double a) {
    m_spectrumAttributes.T_dead_corr_coef = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setCnt_slow(double a) {
    m_spectrumAttributes.cnt_slow = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setCnt_fast(double a) {
    m_spectrumAttributes.cnt_fast = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setCnt_fast_rst(double a) {
    m_spectrumAttributes.cnt_fast_rst = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setT_dead_percents_est1(double a) {
    m_spectrumAttributes.T_dead_percents_est1 = a; //TODO: value validator
}

void ctrl::SpectrumSPM::setT_dead_percents_est2(double a) {
    m_spectrumAttributes.T_dead_percents_est2 = a; //TODO: value validator
}
