#include "sensorhandler.h"
#include <QDebug>
#include <QtMath>

SensorHandler::SensorHandler(QObject *parent)
    : QObject(parent),
      startX(0.0), startY(0.0), startZ(0.0)
{
    gyroscopeHandler = new GyroscopeHandler();
    accelerometerHandler = new AccelerometerHandler();

    connect(accelerometerHandler, &AccelerometerHandler::pathDataReady, this, &SensorHandler::handlePathData);
}

void SensorHandler::handlePathData(double deltaX, double deltaY, double deltaZ)
{
    double angleZ = gyroscopeHandler->getAngleZ();
    addNewPath(deltaX, deltaY, deltaZ, angleZ);
    if (!paths.isEmpty())
        qDebug() << paths.last().toString();
    printAllPaths();
}

void SensorHandler::addNewPath(double deltaX, double deltaY, double deltaZ, double angleZ)
{

    // if (gyroscopeHandler->isMoving()) {
    //     qDebug() << "Skipping path addition because the gyroscope indicates movement.";
    //     return;
    // }

    double maxDelta = (qAbs(deltaX) > qAbs(deltaY)) ? qAbs(deltaX) : qAbs(deltaY);
    if (maxDelta <= 0.02)
    {
        qDebug() << "Skipping path addition because the gyroscope indicates movement.";
        return;
    }

    QDateTime currentTime = QDateTime::currentDateTime();

    // Check if the time since the last path is less than 1 second
    if (!lastPathTime.isNull() && lastPathTime.msecsTo(currentTime) < 2000)
    {
        qDebug() << "Skipping path due to short interval.";
        return;
    }

    double adjustedEndX, adjustedEndY;
    QString direction;
    if (angleZ >= -45 && angleZ < 45)
    {
        if (qAbs(deltaX) > qAbs(deltaY))
        {
            direction = (deltaX > 0) ? "right" : "left";
            adjustedEndX = deltaX + startX;
            adjustedEndY = startY;
        }
        else
        {
            direction = (deltaY < 0) ? "bottom" : "top";
            adjustedEndX = startX;
            adjustedEndY = deltaY + startY;
        }
    }
    else if (angleZ >= 45 && angleZ < 135)
    {
        if (qAbs(deltaX) > qAbs(deltaY))
        {
            direction = (deltaX < 0) ? "bottom" : "top";
            adjustedEndX = startX;
            adjustedEndY = deltaX + startY;
        }
        else
        {
            direction = (deltaY < 0) ? "right" : "left";
            adjustedEndX = (-deltaY) + startX;
            adjustedEndY = startY;
        }
    }
    else if (angleZ >= 135 || angleZ < -135)
    {
        if (qAbs(deltaX) > qAbs(deltaY))
        {
            direction = (deltaX < 0) ? "right" : "left";
            adjustedEndX = (-deltaX) + startX;
            adjustedEndY = startY;
        }
        else
        {
            direction = (0 < deltaY) ? "bottom" : "top";
            adjustedEndX = startX;
            adjustedEndY = (-deltaY) + startY;
        }
    }
    else
    {
        if (qAbs(deltaX) > qAbs(deltaY))
        {
            direction = (0 < deltaX) ? "bottom" : "top";
            adjustedEndX = startX;
            adjustedEndY = (-deltaX) + startY;
        }
        else
        {
            direction = (0 < deltaY) ? "right" : "left";
            adjustedEndX = deltaY + startX;
            adjustedEndY = startY;
        }
    }
    paths.append(Path(startX, startY, startZ, adjustedEndX, adjustedEndY, deltaZ, angleZ, direction));
    emit newPathAdded(paths);
    startX = adjustedEndX;
    startY = adjustedEndY;
    lastPathTime = currentTime;
}

void SensorHandler::printAllPaths() const
{
    qDebug() << "All Paths:";
    for (const Path &path : paths)
    {
        qDebug() << path.toString();
    }
}

void SensorHandler::reset()
{
    paths.clear();
    startX = 0.0;
    startY = 0.0;
    startZ = 0.0;
    lastPathTime = QDateTime();
    gyroscopeHandler->reset();
    accelerometerHandler->reset();
}

void SensorHandler::start()
{
    gyroscopeHandler->start();
    accelerometerHandler->start();
}

void SensorHandler::stop()
{
    gyroscopeHandler->stop();
    accelerometerHandler->stop();
}

QVector<Path> SensorHandler::getPaths() const { return paths; }
