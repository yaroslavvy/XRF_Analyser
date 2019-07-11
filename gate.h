#ifndef QUALITATIVE_ANALYSIS_GATE_H
#define QUALITATIVE_ANALYSIS_GATE_H

#include <QString>

namespace ctrl {
    class Gate {
    public:
        Gate();

        void readFromString(const QString& line);
        void writeToString(const QString& line) const;

        void setEnergyThreshholds(double energyLowThreshhold, double energyHighThreshhold);

        double getEnergyLowThreshhold() const;
        double getEnergyHighThreshhold() const;

        void setGateName(const QString &gateName);

        const QString& getGateName() const;

    private:
        double m_energyLowThreshhold = 0.0;
        double m_energyHighThreshhold = 0.0;
        QString m_gateName = "New gate";
    };
}

#endif // QUALITATIVE_ANALYSIS_GATE_H
