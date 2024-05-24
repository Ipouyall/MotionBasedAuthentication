#ifndef ACCELEROMETERHANDLER_H
#define ACCELEROMETERHANDLER_H

#include <QObject>
#include <QtSensors/QAccelerometer>
#include <QTimer>
#include <QVector>
#include <QElapsedTimer> // Include QElapsedTimer
#include "kalmanfilter.h"
#include "path.h"
#include "gyroscopehandler.h"

class AccelerometerHandler : public QObject
{
    Q_OBJECT
public:
    AccelerometerHandler();
    void printAllPaths() const;

private slots:
    void readSensorValues();
    void calculateBias();

private:
    QAccelerometer *accelerometer;
    QTimer *timer;
    QVector<double> xReadings, yReadings, zReadings;
    double xBias, yBias, zBias;
    bool isCalibrating;
    const double threshold = 0.1; // Threshold for accelerometer changes
    const double velocityThreshold = 0.01; // Threshold for velocity

    // For distance calculation
    int zeroAccelartionCounter;
    double velocityX, velocityY, velocityZ;
    double distanceX, distanceY, distanceZ;
    double intervalDistanceX, intervalDistanceY, intervalDistanceZ;
    QElapsedTimer elapsedTimer; // Use an instance of QElapsedTimer
    bool wasMoving;
    KalmanFilter kalmanX, kalmanY, kalmanZ;

    // Store paths
    QVector<Path> paths;
    double startX, startY, startZ;

    // Gyroscope handler for circular movement
    GyroscopeHandler *gyroscopeHandler;

    void addNewPath(double endX, double endY, double endZ, double angleZ);
};

#endif // ACCELEROMETERHANDLER_H
