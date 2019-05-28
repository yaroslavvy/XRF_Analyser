#ifndef SPECTRUMCHART_H
#define SPECTRUMCHART_H

#include <QObject>
#include <QChart>
#include <QColor>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QLineSeries>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QAbstractSeries>
#include <QCursor>
#include "spectrumpenstruct.h"
#include "spectrumlistmodel.h"

class SpectrumChart : public QtCharts::QChart {
    Q_OBJECT
public:
    SpectrumChart(QGraphicsItem* parent = nullptr);
    void addSpectrum(const SpectrumPenStruct&, bool resizeAxis = true);
    void setModelSpectrums(SpectrumListModel*);
    void setFullSizeSpectrumArea();

    SpectrumListModel* getModelSpectrums();

    bool setXMode(int xMode);
    int getXMode();
    bool setYMode(int yMode);
    int getYMode();


public slots:
    void recoverAxisLimits();
    void slotUpdateSpectrums(bool);
    void slotCursorMode(int);
    void setAndRepaintMouseCursor(QPointF);


protected:
    virtual void wheelEvent(QGraphicsSceneWheelEvent* event);

private:
    void controlAxisLimits(double maxIntensity, double minValX, double maxValX, bool resizeAxis = true);

    SpectrumListModel* modelSpec;

    int xMode;
    int yMode;

    int modeCursor;
    QPointF posMouse;

    double fullViewMinX;
    double fullViewMaxX;
    double fullViewMinY;
    double fullViewMaxY;

    QtCharts::QLineSeries* verticalLineCursor;
    QtCharts::QLineSeries* horizontalLineCursor;

    QtCharts::QValueAxis* axisSpecX;
};

#endif // SPECTRUMCHART_H
