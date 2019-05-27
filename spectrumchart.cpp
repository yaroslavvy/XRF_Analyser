#include "spectrumchart.h"

SpectrumChart::SpectrumChart(QGraphicsItem* pGraphItem)
    : QtCharts::QChart(pGraphItem),
      xMode(0),
      yMode(0),
      fullViewMinX(0),
      fullViewMaxX(0),
      fullViewMinY(0),
      fullViewMaxY(0)
{
    legend()->hide();
    setMargins(QMargins(0, 0, 0, 0));
    setTitle("Spectrums");

    axisSpecX = new QtCharts::QValueAxis;
    axisSpecX->setRange(0, 1);
    axisSpecX->setTitleText("Energy, KeV");
    axisSpecX->setLabelFormat("%.2f");
    axisSpecX->setTickCount(11);
    axisSpecX->applyNiceNumbers();
    addAxis(axisSpecX, Qt::AlignBottom);
    fullViewMinX = 0.0;
    fullViewMaxX = 1.0;
    connect(axisSpecX, SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(recoverAxisLimits()));

    setAxisY(new QtCharts::QValueAxis);
    qobject_cast<QtCharts::QValueAxis*>(axisY())->setRange(0, 100);
    qobject_cast<QtCharts::QValueAxis*>(axisY())->setTitleText("Intesity, cps");
    qobject_cast<QtCharts::QValueAxis*>(axisY())->setLabelFormat("%.2f");
    qobject_cast<QtCharts::QValueAxis*>(axisY())->setTickCount(6);
    qobject_cast<QtCharts::QValueAxis*>(axisY())->applyNiceNumbers();
    fullViewMinY = 0.0;
    fullViewMaxY = 100.0;
    connect(qobject_cast<QtCharts::QValueAxis*>(axisY()), SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(recoverAxisLimits()));
}

bool SpectrumChart::setXMode(int mode)
{
    if(mode < 0 || mode > 3)
    {
        return false;
    }

    if(mode == xMode)
    {
        return true;
    }

    switch (mode)
    {
        case 0:
            axisSpecX->setRange(0, 1);
            axisSpecX->setTitleText("Energy, KeV");
            axisSpecX->setLabelFormat("%.2f");
            fullViewMinX = 0.0;
            fullViewMaxX = 1.0;
            break;
        case 1:
            axisSpecX->setRange(0, 64);
            axisSpecX->setTitleText("Channels");
            axisSpecX->setLabelFormat("%i");
            fullViewMinX = 0.0;
            fullViewMaxX = 64;
            break;
        case 2:
            axisSpecX->setRange(0, 1);
            axisSpecX->setTitleText("Wave length, nm");
            axisSpecX->setLabelFormat("%.3f");
            fullViewMinX = 0.0;
            fullViewMaxX = 1.0;
            break;
        case 3:
            axisSpecX->setRange(0, 10);
            axisSpecX->setTitleText("Wave length, A");
            axisSpecX->setLabelFormat("%.2f");
            fullViewMinX = 0.0;
            fullViewMaxX = 10.0;
            break;
        default:
            return false;
    }

    xMode = mode;
    slotUpdateSpectrums(true);
    return true;
}

int SpectrumChart::getXMode()
{
    return xMode;
}

bool SpectrumChart::setYMode(int mode)
{
    if(mode < 0 || mode > 1)
    {
        return false;
    }

    if(mode == yMode)
    {
        return true;
    }

    switch (yMode)
    {
        case 0:
            disconnect(qobject_cast<QtCharts::QValueAxis*>(axisY()), SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(recoverAxisLimits()));
            break;
        case 1:
            disconnect(qobject_cast<QtCharts::QLogValueAxis*>(axisY()), SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(recoverAxisLimits()));
            break;
        default:
            break;
    }

    removeAxis(axisY());

    switch (mode)
    {
        case 0:
            setAxisY(new QtCharts::QValueAxis);
            qobject_cast<QtCharts::QValueAxis*>(axisY())->setRange(0, 100);
            qobject_cast<QtCharts::QValueAxis*>(axisY())->setTitleText("Intesity, cps");
            qobject_cast<QtCharts::QValueAxis*>(axisY())->setLabelFormat("%.2f");
            qobject_cast<QtCharts::QValueAxis*>(axisY())->setTickCount(6);
            qobject_cast<QtCharts::QValueAxis*>(axisY())->applyNiceNumbers();
            connect(qobject_cast<QtCharts::QValueAxis*>(axisY()), SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(recoverAxisLimits()));
            fullViewMinY = 0.0;
            fullViewMaxY = 100.0;
            break;
        case 1:
            setAxisY(new QtCharts::QLogValueAxis);
            qobject_cast<QtCharts::QLogValueAxis*>(axisY())->setRange(0.00001, 100);//min value of the log axisY can not be ZERO or NEGATIVE
            qobject_cast<QtCharts::QLogValueAxis*>(axisY())->setTitleText("Intesity, cps");
            qobject_cast<QtCharts::QLogValueAxis*>(axisY())->setLabelFormat("%.0E");
            connect(qobject_cast<QtCharts::QLogValueAxis*>(axisY()), SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(recoverAxisLimits()));
            fullViewMinY = 0.00001;//min value of the log axisY can not be ZERO or NEGATIVE
            fullViewMaxY = 100.0;
            break;
        default:
            return false;
    }

    yMode = mode;

    slotUpdateSpectrums(true);
    return true;
}

int SpectrumChart::getYMode()
{
    return yMode;
}

void SpectrumChart::slotUpdateSpectrums(bool resizeAxis)
{
    removeAllSeries();
    QList<SpectrumPenStruct> tmpSpecLst = modelSpec->getSpecList();
    foreach(SpectrumPenStruct specPenStruct, tmpSpecLst)
    {
        addSpectrum(specPenStruct, resizeAxis);
    }
    if(resizeAxis)
    {
        axisSpecX->setRange(fullViewMinX, fullViewMaxX);
    }

    switch (yMode)
    {
        case 0:
            if(resizeAxis)
            {
                qobject_cast<QtCharts::QValueAxis*>(axisY())->setRange(fullViewMinY, fullViewMaxY);
            }
            break;
        case 1:
            if(resizeAxis)
            {
                qobject_cast<QtCharts::QLogValueAxis*>(axisY())->setRange(fullViewMinY, fullViewMaxY);
            }
            break;
        default:
            break;
    }
}

void SpectrumChart::addSpectrum(const SpectrumPenStruct &specPenStruct, bool resizeAxis)
{
    QtCharts::QLineSeries* series = new QtCharts::QLineSeries;
    series->setPen(specPenStruct.pen);//QPen(Qt::red, 5, Qt::DashLine)
    QVector<double> vecIntensities;

    int channel = 0.0;
    double startSpec = specPenStruct.spm.getStartSpectrum();
    double step = specPenStruct.spm.getStep();

    double maxIntensity = 0.0;

    if(yMode == 1)
    {
        maxIntensity = 0.00001;//min value of the log axisY can not be ZERO or NEGATIVE
    }

    double minValX = 0.0;
    double maxValX = 0.0;

    double valX = 0.0;

    bool firstXValue = true;

    vecIntensities = specPenStruct.spm.getDataSpm();
    firstXValue = true;
    foreach(double valY, vecIntensities)
    {
        switch (xMode)
        {
            case 0:
                valX = startSpec + (channel * step);//KeV
                break;
            case 1:
                valX = channel;//channels
                break;
            case 2:
                valX = 1.239842 / (startSpec + (channel * step));//nm
                if(valX < 0)
                {
                    channel++;
                    continue;
                }
                break;
            case 3:
               valX = 12.39842 / (startSpec + (channel * step));//A
               if(valX < 0)
               {
                   channel++;
                   continue;
               }
               break;
        }
        if(firstXValue)
        {
            minValX = valX;
            firstXValue = false;
        }

        if(valX < minValX)
        {
            minValX = valX;
        }

        if(valX > maxValX)
        {
            maxValX = valX;
        }

        if(valY > maxIntensity)
        {
            maxIntensity = valY;
        }

        if((yMode == 1) && (valY < 0.00001 ))//min value of the log axisY can not be ZERO or NEGATIVE
        {
            valY = 0.00001;
        }
        series->append(valX, valY);
        channel++;
    }

    addSeries(series);

    series->attachAxis(axisSpecX);

    switch (yMode)
    {
        case 0:
            series->attachAxis(qobject_cast<QtCharts::QValueAxis*>(axisY()));
            break;
        case 1:
            series->attachAxis(qobject_cast<QtCharts::QLogValueAxis*>(axisY()));
            break;
        default:
            break;
    }

    controlAxisLimits(maxIntensity, minValX, maxValX, resizeAxis);
}

void SpectrumChart::mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)
{
    axisSpecX->setRange(fullViewMinX, fullViewMaxX);
    switch (yMode)
    {
        case 0:
            qobject_cast<QtCharts::QValueAxis*>(axisY())->setRange(fullViewMinY, fullViewMaxY);
            break;
        case 1:
            qobject_cast<QtCharts::QLogValueAxis*>(axisY())->setRange(fullViewMinY, fullViewMaxY);
            break;
        default:
            break;
    }
}

void SpectrumChart::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    double factor = 1.5;
    if(event->delta() > 0)
    {
        QRectF plotAreaChart (plotArea());
        QPointF mousePos (event->scenePos());
        double factorIn = factor;
        double leftNewRect = (mousePos.x() - (mousePos.x() - plotAreaChart.left()) / factorIn);
        double topNewRect = (mousePos.y() - (mousePos.y() - plotAreaChart.top()) / factorIn);
        double widthNewRect = (plotAreaChart.right() - plotAreaChart.left()) / factorIn;
        double heightNewRect = (plotAreaChart.bottom() - mousePos.y()) + ((mousePos.y() - plotAreaChart.top()) / factorIn);
        QRectF newRect (leftNewRect, topNewRect, widthNewRect, heightNewRect);
        zoomIn(newRect);
    }
    else
    {
        QRectF plotAreaChart (plotArea());
        QPointF mousePos (event->scenePos());
        double factorOut = 1 / factor;
        double leftNewRect = (mousePos.x() - (mousePos.x() - plotAreaChart.left()) / factorOut);
        double topNewRect = (mousePos.y() - (mousePos.y() - plotAreaChart.top()) / factorOut);
        double widthNewRect = (plotAreaChart.right() - plotAreaChart.left()) / factorOut;
        double heightNewRect = (plotAreaChart.bottom() - mousePos.y()) + ((mousePos.y() - plotAreaChart.top()) / factorOut);
        QRectF newRect (leftNewRect, topNewRect, widthNewRect, heightNewRect);
        zoomIn(newRect);
    }
    recoverAxisLimits();
}

void SpectrumChart::controlAxisLimits(double maxIntensity, double minValX, double maxValX, bool resizeAxis)
{
    resizeAxis = true;
    switch (yMode)
    {
        case 0:
            if(qobject_cast<QtCharts::QValueAxis*>(axisY())->max() < (1.1 * maxIntensity))
            {
                fullViewMinY = 0.0;
                fullViewMaxY = maxIntensity * 1.1;
                if(resizeAxis)
                {
                    qobject_cast<QtCharts::QValueAxis*>(axisY())->setMax(maxIntensity * 1.1);
                }
            }
            break;
        case 1:
            if(qobject_cast<QtCharts::QLogValueAxis*>(axisY())->max() < (1.1 * maxIntensity))
            {
                fullViewMinY = 0.00001;//min value of the log axisY can not be ZERO or NEGATIVE
                fullViewMaxY = maxIntensity * 10;
                if(resizeAxis)
                {
                    qobject_cast<QtCharts::QLogValueAxis*>(axisY())->setMax(maxIntensity * 10);
                }
            }
            break;
        default:
            break;
    }

    if((axisSpecX->min() > minValX) && (minValX > 0))
    {
        switch (xMode)
        {
            case 2:
                if(minValX < 0.02478)
                {
                    fullViewMinX = 0.02478;//strict order because connect(axisSpecX, SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(recoverAxisLimits()));
                    if(resizeAxis)
                    {
                        axisSpecX->setMin(0.02478);
                    }
                }
                else
                {
                    fullViewMinX = minValX;
                    if(resizeAxis)
                    {
                        axisSpecX->setMin(minValX);
                    }
                }
                break;

            case 3:
                if(minValX < 0.2478)
                {
                    fullViewMinX = 0.2478;
                    if(resizeAxis)
                    {
                        axisSpecX->setMin(0.2478);
                    }
                }
                else
                {
                    fullViewMinX = minValX;
                    if(resizeAxis)
                    {
                        axisSpecX->setMin(minValX);
                    }
                }
                break;

            default:
                fullViewMinX = minValX;
                if(resizeAxis)
                {
                    axisSpecX->setMin(minValX);
                }
                break;
        }
    }

    if(axisSpecX->max() < maxValX)
    {
        switch (xMode)
        {
            case 2:
                if(maxValX > 2.4796)
                {
                    fullViewMaxX = 2.4796;
                    if(resizeAxis)
                    {
                        axisSpecX->setMax(2.4796);
                    }
                }
                else
                {
                    fullViewMaxX = maxValX;
                    if(resizeAxis)
                    {
                        axisSpecX->setMax(maxValX);
                    }
                }
                break;

            case 3:
                if(maxValX > 24.796)
                {
                    fullViewMaxX = 24.796;
                    if(resizeAxis)
                    {
                        axisSpecX->setMax(24.796);
                    }
                }
                else
                {
                    fullViewMaxX = maxValX;
                    if(resizeAxis)
                    {
                        axisSpecX->setMax(maxValX);
                    }
                }
                break;

            default:
                fullViewMaxX = maxValX;
                if(resizeAxis)
                {
                    axisSpecX->setMax(maxValX);
                }
                break;
        }
    }
    return;
}

void SpectrumChart::recoverAxisLimits()
{
    if(axisSpecX->min() < fullViewMinX)
    {
        axisSpecX->setMin(fullViewMinX);
    }

    if(axisSpecX->max() > fullViewMaxX)
    {
        axisSpecX->setMax(fullViewMaxX);
    }

    switch (yMode)
    {
        case 0:
            if(qobject_cast<QtCharts::QValueAxis*>(axisY())->min() < fullViewMinY)
            {
                qobject_cast<QtCharts::QValueAxis*>(axisY())->setMin(fullViewMinY);
            }

            if(qobject_cast<QtCharts::QValueAxis*>(axisY())->max() > fullViewMaxY)
            {
                qobject_cast<QtCharts::QValueAxis*>(axisY())->setMax(fullViewMaxY);
            }
            break;
        case 1:
            if(qobject_cast<QtCharts::QLogValueAxis*>(axisY())->min() < fullViewMinY)
            {
                qobject_cast<QtCharts::QLogValueAxis*>(axisY())->setMin(fullViewMinY);
            }

            if(qobject_cast<QtCharts::QLogValueAxis*>(axisY())->max() > fullViewMaxY)
            {
                qobject_cast<QtCharts::QLogValueAxis*>(axisY())->setMax(fullViewMaxY);
            }
            break;
        default:
            break;
    }

    return;
}

void SpectrumChart::setModelSpectrums(SpectrumListModel* mdl)
{
    modelSpec = mdl;
    slotUpdateSpectrums(true);
    connect(modelSpec, SIGNAL(updateSpectrums(bool)), this, SLOT(slotUpdateSpectrums(bool)));
    return;
}

SpectrumListModel* SpectrumChart::getModelSpectrums()
{
    return modelSpec;
}
