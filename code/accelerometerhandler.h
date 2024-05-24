#ifndef ACCELEROMETERHANDLER_H
#define ACCELEROMETERHANDLER_H

#include <QObject>
#include <QtSensors/QAccelerometer>
#include <QTimer>
#include <QVector>
#include <QElapsedTimer> // Include QElapsedTimer
#include "kalmanfilter.h"

class AccelerometerHandler : public QObject
{
    Q_OBJECT
public:
    AccelerometerHandler();

private slots:
    void readSensorValues();
    void calculateBias();

private:
    QAccelerometer *accelerometer;
    QTimer *timer;
    QVector<double> xReadings, yReadings, zReadings;
    double xBias, yBias, zBias;
    bool isCalibrating;
    const double threshold = 0.6; // Threshold for accelerometer changes
    const double velocityThreshold = 0.01; // Threshold for velocity

    // For distance calculation
    int zeroAccelartionCounter;
    double velocityX, velocityY, velocityZ;
    double distanceX, distanceY, distanceZ;
    double intervalDistanceX, intervalDistanceY, intervalDistanceZ;
    QElapsedTimer elapsedTimer; // Use an instance of QElapsedTimer
    bool wasMoving;
    KalmanFilter kalmanX, kalmanY, kalmanZ;
};

#endif // ACCELEROMETERHANDLER_H
