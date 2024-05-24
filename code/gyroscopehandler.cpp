#include "GyroscopeHandler.h"
#include <QDebug>

GyroscopeHandler::GyroscopeHandler()
    : angularVelocityX(0.0), angularVelocityY(0.0), angularVelocityZ(0.0),
    angleX(0.0), angleY(0.0), angleZ(0.0)
{
    // Create a gyroscope sensor
    gyroscope = new QGyroscope(this);
    gyroscope->start();

    // Create a timer for periodic reading
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GyroscopeHandler::readGyroscopeValues);
    timer->start(200); // 200 milliseconds interval

    // Start the elapsed timer
    elapsedTimer.start();
}

GyroscopeHandler::~GyroscopeHandler()
{
    // Print angular displacement on destruction
    printAngularDisplacement();
}

void GyroscopeHandler::readGyroscopeValues()
{
    // Get the current reading
    QGyroscopeReading *reading = gyroscope->reading();
    if (reading) {
        // Read angular velocity
        double omegaX = reading->x();
        double omegaY = reading->y();
        double omegaZ = reading->z();

        // Apply threshold
        if (qAbs(omegaX) < threshold) omegaX = 0.0;
        if (qAbs(omegaY) < threshold) omegaY = 0.0;
        if (qAbs(omegaZ) < threshold) omegaZ = 0.0;

        // Calculate time interval
        double deltaTime = elapsedTimer.elapsed() / 1000.0; // convert to seconds

        // Restart the elapsed timer
        elapsedTimer.restart();

        // Update angular velocity
        angularVelocityX = omegaX;
        angularVelocityY = omegaY;
        angularVelocityZ = omegaZ;

        // Update angular displacement (integrate angular velocity)
        angleX += angularVelocityX * deltaTime;
        angleY += angularVelocityY * deltaTime;
        angleZ += angularVelocityZ * deltaTime;

        // Check if velocity is zero
        if (omegaX == 0.0 && omegaY == 0.0 && omegaZ == 0.0) {
            zeroVelocityCounter++;
            if (wasMoving && zeroVelocityCounter >= 5) {
                roundAnglesToNearest90();
                printAngularDisplacement();
                zeroVelocityCounter = 0; // Reset the counter
                wasMoving = false; // Reset wasMoving after rounding
            }
        } else {
            zeroVelocityCounter = 0; // Reset the counter if velocity is non-zero
            wasMoving = true; // Set wasMoving to true if there is any movement
        }

        // qDebug() << "Gyroscope values - OmegaX:" << omegaX
        //          << " OmegaY:" << omegaY
        //          << " OmegaZ:" << omegaZ;
        // qDebug() << "Angular Displacement - AngleX:" << angleX
        //          << " AngleY:" << angleY
        //          << " AngleZ:" << angleZ;
    }
}

void GyroscopeHandler::printAngularDisplacement() const
{
    qDebug() << "Final Angular Displacement:";
    qDebug() << "AngleX:" << angleX;
    qDebug() << "AngleY:" << angleY;
    qDebug() << "AngleZ:" << angleZ;
}


void GyroscopeHandler::roundAnglesToNearest90()
{
    // auto roundToNearest90 = [](double angle) {
    //     double remainder = std::fmod(angle, 90.0);
    //     if (remainder < 0) remainder += 90.0; // Handle negative angles
    //     if (remainder < 45.0) {
    //         return static_cast<int>(angle - remainder);
    //     } else {
    //         return static_cast<int>(angle - remainder + 90.0);
    //     }
    // };

    angleX = roundTooNearest90(angleX);
    angleY = roundTooNearest90(angleY);
    angleZ = roundTooNearest90(angleZ);

    qDebug() << "Rounded Angles to nearest 90 degrees - AngleX:" << angleX
             << " AngleY:" << angleY
             << " AngleZ:" << angleZ;
}

double GyroscopeHandler::roundTooNearest90(double angle) const {
    double remainder = std::fmod(angle, 90.0);
    if (remainder < 0) remainder += 90.0; // Handle negative angles
    if (remainder < 45.0) {
        return static_cast<int>(angle - remainder);
    } else {
        return static_cast<int>(angle - remainder + 90.0);
    }
}

double GyroscopeHandler::getAngleZ() const
{
    return roundTooNearest90(angleZ);
}

