#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>
#include <string>
#include <algorithm>
#include <QColor>
#include <QVector>
#include <QPen>
#include <QBrush>

class Service : public QObject
{
    Q_OBJECT
public:
    explicit Service(QObject *parent = nullptr);

    static bool isValidIntFromStr(const std::string &possibleInteger);
    static bool isValidDoubleFromStr(const std::string &possibleDouble);
    static const std::string commaToDot (const std::string &string);
    static const QPen getPenForSpec (int orderNumberLoadedSpectrum);
};

#endif // SERVICE_H
