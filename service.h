#ifndef QUALITATIVE_ANALYSIS_SERVICE_H
#define QUALITATIVE_ANALYSIS_SERVICE_H

#include <QObject>
#include <string>
class QPen;
class QWidget;

namespace ui {
    class MainWindow;
}

namespace srvcSpec {
    const std::string commaToDot (std::string str);
    const QPen getPenForSpec (int orderNumberLoadedSpectrum, int penWidth);
}

#endif // QUALITATIVE_ANALYSIS_SERVICE_H
