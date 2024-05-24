#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include "gyroscopehandler.h"
#include "accelerometerhandler.h"
#include "path.h"

class SensorHandler : public QObject
{
    Q_OBJECT

public:
    SensorHandler(QObject *parent = nullptr);
    void reset(); // Method to reset the handler
    void start(); // Method to start the handler
    void stop();  // Method to stop the handler
    QVector<Path> getPaths() const;
    void printAllPaths() const;

private slots:
    void handlePathData(double deltaX, double deltaY, double deltaZ);

private:
    void addNewPath(double deltaX, double deltaY, double deltaZ, double angleZ);


    GyroscopeHandler *gyroscopeHandler;
    AccelerometerHandler *accelerometerHandler;

    QVector<Path> paths;

    double startX;
    double startY;
    double startZ;
    QDateTime lastPathTime;
};

#endif // SENSORHANDLER_H
