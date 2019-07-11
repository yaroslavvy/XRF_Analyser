#include "gate.h"
#include <QFile>
#include <QFileInfo>
#include <QTranslator>
#include "exception.h"
#include "service.h"

ctrl::Gate::Gate() {
}

void ctrl::Gate::readFromString(const QString& line) {
    QStringList strLst = line.split(';');
    if (strLst.size() != 3) {
        throw Exception(QTranslator::tr(" Opening is failed. Gate is wrong\n"));
    }
    setGateName(strLst.takeFirst());
    double possibleEnergyLowThreshhold = std::stod(srvcSpec::commaToDot(strLst.takeFirst().toStdString()));
    double possibleEnergyHighThreshhold = std::stod(srvcSpec::commaToDot(strLst.takeFirst().toStdString()));
    setEnergyThreshholds(possibleEnergyLowThreshhold, possibleEnergyHighThreshhold);
}

void ctrl::Gate::writeToString(const QString& line) const {
    (void)line;
}

void ctrl::Gate::setEnergyThreshholds(double energyLowThreshhold, double energyHighThreshhold) {
    if ((energyLowThreshhold <= energyHighThreshhold) && (energyLowThreshhold >= 0)) {
        m_energyLowThreshhold = energyLowThreshhold;
        m_energyHighThreshhold = energyHighThreshhold;
    }
    else {
        m_energyLowThreshhold = 0.0;
        m_energyHighThreshhold = 0.0;
    }
}

double ctrl::Gate::getEnergyLowThreshhold() const {
    return m_energyLowThreshhold;
}

double ctrl::Gate::getEnergyHighThreshhold() const {
    return m_energyHighThreshhold;
}

void ctrl::Gate::setGateName(const QString &gateName) {
    m_gateName = gateName;
}

const QString& ctrl::Gate::getGateName() const {
    return m_gateName;
}
