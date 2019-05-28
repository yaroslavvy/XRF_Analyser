#include "spectrumspm.h"

SpectrumSPM::SpectrumSPM(QObject *parent)
    : QObject(parent) {
    startSpectrum = -1;
    finishSpectrum = -1;
    step = -1;
    amperageTube = -1;
    voltageTube = -1;
    exposureSec = -1;
    filter = -1;
    T_dead_prc = -1;
    T_peak_us = -1;
    gain = -1;
    th = -1;
    sp_serial  = -1;
    T_peak_s_us  = -1;
    T_peak_f_ns  = -1;
    th_fast  = -1;
    th_slow  = -1;
    th_LLD  = -1;
    PUR = false;
    T_dead_corr_coef  = -1;
    cnt_slow = -1;
    cnt_fast  = -1;
    cnt_fast_rst  = -1;
    T_dead_prc_est1  = -1;
    T_dead_prc_est2  = -1;
}

SpectrumSPM::SpectrumSPM(const SpectrumSPM &spec, QObject *parent)
    : QObject(parent) {
    setSpectrumName(spec.getSpectrumName());
    setDateTime(spec.getDateTime());
    setDataSpm(spec.getDataSpm());
    setStartSpectrum(spec.getStartSpectrum());
    setFinishSpectrum(spec.getFinishSpectrum());
    setStep(spec.getStep());
    setAmperageTube(spec.getAmperageTube());
    setVoltageTube(spec.getVoltageTube());
    setExposureSec(spec.getExposureSec());
    setFilter(spec.getFilter());
    setT_dead_prc(spec.getT_dead_prc());
    setT_peak_us(spec.getT_peak_us());
    setGain(spec.getGain());
    setTh(spec.getTh());
    setSp_type(spec.getSp_type());
    setSp_serial(spec.getSp_serial());
    setT_peak_s_us(spec.getT_peak_s_us());
    setT_peak_f_ns(spec.getT_peak_f_ns());
    setTh_fast(spec.getTh_fast());
    setTh_slow(spec.getTh_slow());
    setTh_LLD(spec.getTh_LLD());
    setPUR(spec.getPUR());
    setT_dead_corr_coef(spec.getT_dead_corr_coef());
    setCnt_slow(spec.getCnt_slow());
    setCnt_fast(spec.getCnt_fast());
    setCnt_fast_rst(spec.getCnt_fast_rst());
    setT_dead_prc_est1(spec.getT_dead_prc_est1());
    setT_dead_prc_est2(spec.getT_dead_prc_est2());
}

const SpectrumSPM& SpectrumSPM::operator= (const SpectrumSPM &spec) {
    if(&spec != this) {
        setSpectrumName(spec.getSpectrumName());
        setDateTime(spec.getDateTime());
        setDataSpm(spec.getDataSpm());
        setStartSpectrum(spec.getStartSpectrum());
        setFinishSpectrum(spec.getFinishSpectrum());
        setStep(spec.getStep());
        setAmperageTube(spec.getAmperageTube());
        setVoltageTube(spec.getVoltageTube());
        setExposureSec(spec.getExposureSec());
        setFilter(spec.getFilter());
        setT_dead_prc(spec.getT_dead_prc());
        setT_peak_us(spec.getT_peak_us());
        setGain(spec.getGain());
        setTh(spec.getTh());
        setSp_type(spec.getSp_type());
        setSp_serial(spec.getSp_serial());
        setT_peak_s_us(spec.getT_peak_s_us());
        setT_peak_f_ns(spec.getT_peak_f_ns());
        setTh_fast(spec.getTh_fast());
        setTh_slow(spec.getTh_slow());
        setTh_LLD(spec.getTh_LLD());
        setPUR(spec.getPUR());
        setT_dead_corr_coef(spec.getT_dead_corr_coef());
        setCnt_slow(spec.getCnt_slow());
        setCnt_fast(spec.getCnt_fast());
        setCnt_fast_rst(spec.getCnt_fast_rst());
        setT_dead_prc_est1(spec.getT_dead_prc_est1());
        setT_dead_prc_est2(spec.getT_dead_prc_est2());
    }
    return *this;
}

bool SpectrumSPM::readFromFile(const QString& fileName) {
    if (!QFile::exists(fileName)) {
        qDebug() << fileName + " File not found\n";
        return false;
    }
    QFile file(fileName);
    QFileInfo fileInfo(file);
    if (file.isOpen()) {
        qDebug() << fileInfo.fileName() + " File is already open\n";
        return false;
    }
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << fileInfo.fileName() + " Error file reading\n";
        return false;
    }
    QString str;
    QStringList strLst;
    std::string tmp;
    spm.clear();
    if(!file.atEnd()) {
        if(!setSpectrumName(fileInfo.fileName())){
            return false;
        }
        str = file.readLine();
        strLst = str.split(' ');
        strLst.removeAll("");

        tmp = Service::commaToDot(strLst.takeFirst().toStdString());
        if(Service::isValidDoubleFromStr(tmp)) {
            if(!setStartSpectrum(std::stod(tmp)))
                return false;
            tmp.clear();
        }
        else {
            qDebug() << fileInfo.fileName() + " Invalid value of startSpectrum\n";
            return false;
        }

        tmp = Service::commaToDot(strLst.takeFirst().toStdString());
        if(Service::isValidDoubleFromStr(tmp)) {
            if(!setFinishSpectrum(std::stod(tmp)))
                return false;
            tmp.clear();
        }
        else {
            qDebug() << fileInfo.fileName() + " Invalid value of finishSpectrum\n";
            return false;
        }

        tmp = Service::commaToDot(strLst.takeFirst().toStdString());
        if(Service::isValidDoubleFromStr(tmp)) {
            if(!setStep(std::stod(tmp)))
                return false;
            tmp.clear();
        }
        else {
            qDebug() << fileInfo.fileName() + " Invalid value of step\n";
            return false;
        }

        tmp = strLst.takeFirst().toStdString();
        if(Service::isValidIntFromStr(tmp)) {
            if(!setExposureSec(std::stoi(tmp))){
                return false;
            }
            tmp.clear();
        }
        else {
            qDebug() << fileInfo.fileName() + " Invalid value of exposure\n";
            return false;
        }

        tmp = Service::commaToDot(strLst.takeFirst().toStdString());
        if(Service::isValidDoubleFromStr(tmp)) {
            if(!setVoltageTube(std::stod(tmp))){
                return false;
            }
            tmp.clear();
        }
        else {
            qDebug() << fileInfo.fileName() + " Invalid value of tube voltage\n";
            return false;
        }

        tmp = Service::commaToDot(strLst.takeFirst().toStdString());
        if(Service::isValidDoubleFromStr(tmp)) {
            if(!setAmperageTube(std::stod(tmp))){
                return false;
            }
            tmp.clear();
        }
        else {
            qDebug() << fileInfo.fileName() + " Invalid value of tube amperage\n";
            return false;
        }

        tmp = strLst.takeFirst().toStdString();
        if(Service::isValidIntFromStr(tmp)) {
            if(!setFilter(std::stoi(tmp))){
                return false;
            }
            tmp.clear();
        }
        else {
            qDebug() << fileInfo.fileName() + " Invalid value of filter\n";
            return false;
        }

        strLst.takeFirst(); // What is 0?
        strLst.takeFirst(); // What is 0?
        strLst.takeFirst(); // What is 8?

        QString date(strLst.takeFirst());
        QStringList dateLst(date.split("."));

        tmp = dateLst.takeFirst().toStdString();
        int day;
        if(Service::isValidIntFromStr(tmp)) {
            day = std::stoi(tmp);
            tmp.clear();
        }
        else {
            qDebug() << fileInfo.fileName() + " Invalid value of day\n";
            return false;
        }

        tmp = dateLst.takeFirst().toStdString();
        int month;
        if(Service::isValidIntFromStr(tmp)) {
            month = std::stoi(tmp);
            tmp.clear();
        }
        else {
            qDebug() << fileInfo.fileName() + " Invalid value of month\n";
            return false;
        }

        tmp = dateLst.takeFirst().toStdString();
        int year;
        if(Service::isValidIntFromStr(tmp)) {
            year = std::stoi(tmp);
            tmp.clear();
        }
        else {
            qDebug() << fileInfo.fileName() + " Invalid value of year\n";
            return false;
        }

        QString time(strLst.takeFirst());
        QStringList timeLst(time.split(":"));

        tmp = timeLst.takeFirst().toStdString();
        int hour;
        if(Service::isValidIntFromStr(tmp)) {
            hour = std::stoi(tmp);
            tmp.clear();
        }
        else {
            qDebug() << fileInfo.fileName() + " Invalid value of hour\n";
            return false;
        }

        tmp = timeLst.takeFirst().toStdString();
        int minute;
        if(Service::isValidIntFromStr(tmp)){
            minute = std::stoi(tmp);
            tmp.clear();
        }
        else {
            qDebug() << fileInfo.fileName() + " Invalid value of minute\n";
            return false;
        }

        tmp = timeLst.takeFirst().toStdString();
        int second;
        if(Service::isValidIntFromStr(tmp)) {
            second = std::stoi(tmp);
            tmp.clear();
        }
        else {
            qDebug() << fileInfo.fileName() + " Invalid value of second\n";
            return false;
        }

        QDateTime dT;
        dT.setDate(QDate(year, month, day));
        dT.setTime(QTime(hour, minute, second));
        if(!setDateTime(dT))
            return false;

        //Tags (many many arguments)

        str.clear();
        strLst.clear();
    }
    else {
        qDebug() << fileInfo.fileName() + " Error file reading at the first line\n";
        return false;
    }

    if(file.atEnd()) {
        qDebug() << fileInfo.fileName() + " Error file reading after the first line\n";
        return false;
    }

    while(!file.atEnd()) {
        str = file.readLine();
        strLst = str.split(' ');
        for(QStringList::const_iterator i = strLst.cbegin(); i != strLst.cend(); ++i) {
            if((i->isEmpty()) || (i->end() != std::find(i->begin(), i->end(), "\n") )) {
                continue;
            }
            tmp = Service::commaToDot(i->toStdString());
            if(!Service::isValidDoubleFromStr(tmp)) {
                qDebug() << fileInfo.fileName() + " Invalid value of intensity\n";
                return false;
            }
            spm.push_back(std::stod(tmp));
            tmp.clear();
        }
        str.clear();
        strLst.clear();
    }
    file.close();
    return true;
}

bool SpectrumSPM::writeToFile(const QString& fileName) const {
    return true;
}

bool SpectrumSPM::setDataSpm (const QVector<double> &a) {
    spm = a;
    return true;
}

const QVector<double>& SpectrumSPM::getDataSpm() const {
    return spm;
}

bool SpectrumSPM::setSpectrumName(const QString &a) {
    spectrumName = a;
    return true;
}

const QString& SpectrumSPM::getSpectrumName() const {
    return spectrumName;
}

bool SpectrumSPM::setDateTime(const QDateTime &a) {
    dateTime = a;
    return true;
}

const QDateTime& SpectrumSPM::getDateTime() const {
    return dateTime;
}

bool SpectrumSPM::setStartSpectrum(const double a) {
    startSpectrum = a;
    return true;
}

double SpectrumSPM::getStartSpectrum() const {
    return startSpectrum;
}

bool SpectrumSPM::setFinishSpectrum(const double a) {
    finishSpectrum = a;
    return true;
}

double SpectrumSPM::getFinishSpectrum() const {
    return finishSpectrum;
}

bool SpectrumSPM::setStep(const double a) {
    if(a < 0) {
        qDebug() << getSpectrumName() << ". Step is less than 0 (" + QString::number(a) + ")\n" ;
        return false;
    }
    step = a;
    return true;
}

double SpectrumSPM::getStep() const {
    return step;
}

bool SpectrumSPM::setAmperageTube(const double a) {
    if(a < 0) {
        qDebug() << getSpectrumName() << ". Amperage tube is less than 0 (" + QString::number(a) + ")\n" ;
        return false;
    }
    amperageTube = a;
    return true;
}

double SpectrumSPM::getAmperageTube() const {
    return amperageTube;
}

bool SpectrumSPM::setVoltageTube(const double a) {
    if(a < 0) {
        qDebug() << getSpectrumName() << ". Voltage tube is less than 0 (" + QString::number(a) + ")\n" ;
        return false;
    }
    voltageTube = a;
    return true;
}

double SpectrumSPM::getVoltageTube() const {
    return voltageTube;
}

bool SpectrumSPM::setExposureSec(const int a) {
    if(a < 0) {
        qDebug() << getSpectrumName() << ". Exposure is less than 0 (" + QString::number(a) + " s)\n" ;
        return false;
    }
    exposureSec = a;
    return true;
}

int SpectrumSPM::getExposureSec() const {
    return exposureSec;
}

bool SpectrumSPM::setFilter(const int a) {
    if(a < 0) {
        qDebug() << getSpectrumName() << ". Number filter can not be less than 0 (" + QString::number(a) + ")\n" ;
        return false;
    }
    filter = a;
    return true;
}

int SpectrumSPM::getFilter() const {
    return filter;
}

bool SpectrumSPM::setT_dead_prc(const double a) {
    T_dead_prc = a;
    return true;//Here should be value checking
}

double SpectrumSPM::getT_dead_prc() const {
    return T_dead_prc;
}

bool SpectrumSPM::setT_peak_us(const double a) {
    T_peak_us = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getT_peak_us() const {
    return T_peak_us;
}

bool SpectrumSPM::setGain(const double a) {
    gain = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getGain() const {
    return gain;
}

bool SpectrumSPM::setTh(const double a) {
    th = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getTh() const {
    return th;
}

bool SpectrumSPM::setSp_type(const QString &a) {
    sp_type = a; //Here should be value checking
    return true;
}

const QString& SpectrumSPM::getSp_type() const {
    return sp_type;
}

bool SpectrumSPM::setSp_serial(const int a) {
    sp_serial = a; //Here should be value checking
    return true;
}

int SpectrumSPM::getSp_serial() const {
    return sp_serial;
}

bool SpectrumSPM::setT_peak_s_us(const double a) {
    T_peak_s_us = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getT_peak_s_us() const {
    return T_peak_s_us;
}

bool SpectrumSPM::setT_peak_f_ns(const double a) {
    T_peak_f_ns = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getT_peak_f_ns() const {
    return T_peak_f_ns;
}

bool SpectrumSPM::setTh_fast(const double a) {
    th_fast = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getTh_fast() const {
    return th_fast;
}

bool SpectrumSPM::setTh_slow(const double a) {
    th_slow = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getTh_slow() const {
    return th_slow;
}

bool SpectrumSPM::setTh_LLD(const double a) {
    th_LLD = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getTh_LLD() const {
    return th_LLD;
}

bool SpectrumSPM::setPUR(const bool a) {
    PUR = a; //Here should be value checking
    return true;
}

bool SpectrumSPM::getPUR() const {
    return PUR;
}

bool SpectrumSPM::setT_dead_corr_coef(const double a) {
    T_dead_corr_coef = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getT_dead_corr_coef() const {
    return T_dead_corr_coef;
}

bool SpectrumSPM::setCnt_slow(const double a) {
    cnt_slow = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getCnt_slow() const {
    return cnt_slow;
}

bool SpectrumSPM::setCnt_fast(const double a) {
    cnt_fast = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getCnt_fast() const {
    return cnt_fast;
}

bool SpectrumSPM::setCnt_fast_rst(const double a) {
    cnt_fast_rst = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getCnt_fast_rst() const {
    return cnt_fast_rst;
}

bool SpectrumSPM::setT_dead_prc_est1(const double a) {
    T_dead_prc_est1 = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getT_dead_prc_est1() const {
    return T_dead_prc_est1;
}

bool SpectrumSPM::setT_dead_prc_est2(const double a) {
    T_dead_prc_est2 = a; //Here should be value checking
    return true;
}

double SpectrumSPM::getT_dead_prc_est2() const {
    return T_dead_prc_est2;
}
