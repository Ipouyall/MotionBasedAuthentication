#ifndef GYROSCOPEHANDLER_H
#define GYROSCOPEHANDLER_H

#include <QObject>
#include <QtSensors/QGyroscope>
#include <QTimer>
#include <QElapsedTimer>

class GyroscopeHandler : public QObject
{
    Q_OBJECT
public:
    GyroscopeHandler();
    ~GyroscopeHandler();
    double getAngleZ() const;

    void printAngularDisplacement() const;

private slots:
    void readGyroscopeValues();

private:
    QGyroscope *gyroscope;
    QTimer *timer;
    QElapsedTimer elapsedTimer;

    double angularVelocityX, angularVelocityY, angularVelocityZ;
    double angleX, angleY, angleZ; // Angular displacement

    const double threshold = 0.1; // Threshold for gyroscope changes
    int zeroVelocityCounter = 0; // Counter for zero velocity readings
    bool wasMoving = false; // Indicates if there was movement before stopping

    void roundAnglesToNearest90(); // Method to round angles to nearest multiple of 90
    double roundTooNearest90(double angle) const;
};

#endif // GYROSCOPEHANDLER_H
