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
    setEnergyLowThreshhold(std::stod(srvcSpec::commaToDot(strLst.takeFirst().toStdString())));
    setEnergyHighThreshhold(std::stod(srvcSpec::commaToDot(strLst.takeFirst().toStdString())));
}

void ctrl::Gate::writeToString(const QString& line) const {
    (void)line;
}

void ctrl::Gate::setEnergyLowThreshhold(double energyLowThreshhold) {
    m_energyLowThreshhold = energyLowThreshhold;
}

void ctrl::Gate::setEnergyHighThreshhold(double energyHighThreshhold) {
    m_energyHighThreshhold = energyHighThreshhold;
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
