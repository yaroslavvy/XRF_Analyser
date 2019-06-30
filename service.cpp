#include "service.h"
#include <algorithm>
#include <QColor>
#include <QVector>
#include <QPen>
#include <QBrush>

const std::string srvcSpec::commaToDot (std::string str) {
    std::replace(str.begin(), str.end(), ',', '.');
    return str;
}

const QPen srvcSpec::getPenForSpec(int orderNumberLoadedSpectrum, int penWidth) {
    int color = orderNumberLoadedSpectrum % 21;
    const int penColors[21][3] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {255, 200, 0},
        {255, 0, 255},
        {0, 255, 255},
        {128, 0, 0},
        {0, 128, 0},
        {0, 0, 128},
        {128, 128, 0},
        {128, 0, 128},
        {0, 128, 128},
        {255, 128, 0},
        {255, 0, 128},
        {255, 128, 128},
        {128, 255, 0},
        {0, 255, 128},
        {128, 255, 128},
        {0, 128, 255},
        {128, 0, 255},
        {128, 128, 255}
    };
    return QPen(QBrush(QColor(penColors[color][0], penColors[color][1], penColors[color][2])), penWidth);
}

