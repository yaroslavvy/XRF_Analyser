#include "service.h"

Service::Service(QObject *parent)
    : QObject(parent) {
}

bool Service::isValidIntFromStr(const std::string &possibleInteger) {
    return possibleInteger.find_first_not_of("-+0123456789") == std::string::npos;
}

bool Service::isValidDoubleFromStr(const std::string &possibleDouble) {
    return possibleDouble.find_first_not_of("-+0123456789.") == std::string::npos;
}

const std::string Service::commaToDot (const std::string &string) {
    std::string tmp(string);
    std::replace(tmp.begin(), tmp.end(), ',', '.');
    return tmp;
}

const QPen Service::getPenForSpec (int orderNumberLoadedSpectrum) {
    int defaultWidth = 2;
    switch (orderNumberLoadedSpectrum % 21) {
        case 0:
            return QPen(QBrush(QColor(255, 0, 0)), defaultWidth);
        case 1:
            return QPen(QBrush(QColor(0, 255, 0)), defaultWidth);
        case 2:
            return QPen(QBrush(QColor(0, 0, 255)), defaultWidth);
        case 3:
            return QPen(QBrush(QColor(255, 200, 0)), defaultWidth);
        case 4:
            return QPen(QBrush(QColor(255, 0, 255)), defaultWidth);
        case 5:
            return QPen(QBrush(QColor(0, 255, 255)), defaultWidth);
        case 6:
            return QPen(QBrush(QColor(128, 0, 0)), defaultWidth);
        case 7:
            return QPen(QBrush(QColor(0, 128, 0)), defaultWidth);
        case 8:
            return QPen(QBrush(QColor(0, 0, 128)), defaultWidth);
        case 9:
            return QPen(QBrush(QColor(128, 128, 0)), defaultWidth);
        case 10:
            return QPen(QBrush(QColor(128, 0, 128)), defaultWidth);
        case 11:
            return QPen(QBrush(QColor(0, 128, 128)), defaultWidth);
        case 12:
            return QPen(QBrush(QColor(255, 128, 0)), defaultWidth);
        case 13:
            return QPen(QBrush(QColor(255, 0, 128)), defaultWidth);
        case 14:
            return QPen(QBrush(QColor(255, 128, 128)), defaultWidth);
        case 15:
            return QPen(QBrush(QColor(128, 255, 0)), defaultWidth);
        case 16:
            return QPen(QBrush(QColor(0, 255, 128)), defaultWidth);
        case 17:
            return QPen(QBrush(QColor(128, 255, 128)), defaultWidth);
        case 18:
            return QPen(QBrush(QColor(0, 128, 255)), defaultWidth);
        case 19:
            return QPen(QBrush(QColor(128, 0, 255)), defaultWidth);
        case 20:
            return QPen(QBrush(QColor(128, 128, 255)), defaultWidth);
        default:
            return QPen(QBrush(QColor(255, 0, 0)), defaultWidth);
    }
}

