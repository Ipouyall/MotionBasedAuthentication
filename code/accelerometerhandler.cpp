#include "AccelerometerHandler.h"
#include <QDebug>
#include <QtGlobal>

AccelerometerHandler::AccelerometerHandler()
    : xBias(0.0), yBias(0.0), zBias(0.0), isCalibrating(true),
    velocityX(0.0), velocityY(0.0), velocityZ(0.0),
    distanceX(0.0), distanceY(0.0), distanceZ(0.0),
    intervalDistanceX(0.0), intervalDistanceY(0.0), intervalDistanceZ(0.0),
    wasMoving(false),zeroAccelartionCounter(0),
    kalmanX(0.0, 0.1, 0.1, 1), kalmanY(0.0, 0.1, 0.1, 1), kalmanZ(0.0, 0.1, 0.1, 1)
{
    // Create an accelerometer sensor
    accelerometer = new QAccelerometer(this);
    // accelerometer->setAccelerationMode(QAccelerometer::User);
    accelerometer->start();

    // Create a timer for periodic reading
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &AccelerometerHandler::readSensorValues);
    timer->start(40); // 50 milliseconds interval

    // Set up a timer for calibration
    QTimer::singleShot(4000, this, &AccelerometerHandler::calculateBias); // 4 seconds calibration period

    // Start the elapsed timer
    elapsedTimer.start();
}

void AccelerometerHandler::readSensorValues()
{
    // Get the current reading
    QAccelerometerReading *reading = accelerometer->reading();
    if (reading) {
        if (isCalibrating) {
            // Collect readings for calibration
            xReadings.append(reading->x());
            yReadings.append(reading->y());
            zReadings.append(reading->z());
        } else {
            // Adjust readings by subtracting the bias
            double adjustedX = reading->x() - xBias;
            double adjustedY = reading->y() - yBias;
            double adjustedZ = reading->z() - zBias;

            if (qAbs(adjustedX) < threshold) adjustedX = 0;
            if (qAbs(adjustedY) < threshold) adjustedY = 0;
            if (qAbs(adjustedZ) < threshold) adjustedZ = 0;


            adjustedX = kalmanX.update(adjustedX);
            adjustedY = kalmanY.update(adjustedY);
            adjustedZ = kalmanZ.update(adjustedZ);

            // Apply threshold
            if (qAbs(adjustedX) < threshold) adjustedX = 0;
            if (qAbs(adjustedY) < threshold) adjustedY = 0;
            if (qAbs(adjustedZ) < threshold) adjustedZ = 0;


            // Calculate time interval
            double deltaTime = elapsedTimer.elapsed() / 1000.0; // convert to seconds

            // Restart the elapsed timer
            elapsedTimer.restart();

            qDebug() << "delta time " << deltaTime;

            // Update velocity
            velocityX += adjustedX * deltaTime;
            velocityY += adjustedY * deltaTime;
            velocityZ += adjustedZ * deltaTime;

            qDebug() << "Velocity X: " << velocityX << " Velocity Y: " << velocityY << " Velocity Z: " << velocityZ;
            qDebug() << "acc X: " << adjustedX << " acc Y: " << adjustedY << " acc Z: " << adjustedZ;
            // Update distance
            distanceX += velocityX * deltaTime + 0.5 * adjustedX * deltaTime * deltaTime;
            distanceY += velocityY * deltaTime + 0.5 * adjustedY * deltaTime * deltaTime;
            distanceZ += velocityZ * deltaTime + 0.5 * adjustedZ * deltaTime * deltaTime;

            // Calculate interval distance
            intervalDistanceX += velocityX * deltaTime + 0.5 * adjustedX * deltaTime * deltaTime;
            intervalDistanceY += velocityY * deltaTime + 0.5 * adjustedY * deltaTime * deltaTime;
            intervalDistanceZ += velocityZ * deltaTime + 0.5 * adjustedZ * deltaTime * deltaTime;

            if (adjustedX != 0 || adjustedY != 0 || adjustedZ != 0) {
                // qDebug() << "=========================================================================================";
                // if (wasMoving) {
                //     wasMoving = false;
                // }
                zeroAccelartionCounter = 0;

            }
            if (adjustedX == 0 && adjustedY == 0 && adjustedZ == 0) {
                qDebug() << "=========================================================================================";
                if (zeroAccelartionCounter > 1) {
                    velocityX =0;
                    velocityY =0;
                    velocityZ =0;
                    zeroAccelartionCounter = 0;
                } else {
                    zeroAccelartionCounter++;
                }

            }

            if (qAbs(velocityX) < velocityThreshold && qAbs(velocityY) < velocityThreshold && qAbs(velocityZ) < velocityThreshold) {
                    qDebug() << "Distance between stops - X:" << intervalDistanceX
                             << " Y:" << intervalDistanceY
                             << " Z:" << intervalDistanceZ;
                    intervalDistanceX = 0.0;
                    intervalDistanceY = 0.0;
                    intervalDistanceZ = 0.0;
            }

            qDebug() << "Calibrated Accelerometer values - X:" << adjustedX
                     << " Y:" << adjustedY
                     << " Z:" << adjustedZ;
            qDebug() << "Distance - X:" << distanceX
                     << " Y:" << distanceY
                     << " Z:" << distanceZ;
        }
    }
}

void AccelerometerHandler::calculateBias()
{
    // Calculate the average values for each axis
    int count = xReadings.size();
    if (count > 0) {
        for (int i = 0; i < count; ++i) {
            xBias += xReadings[i];
            yBias += yReadings[i];
            zBias += zReadings[i];
        }
        xBias /= count;
        yBias /= count;
        zBias /= count;
    }

    // Clear the readings as they are no longer needed
    xReadings.clear();
    yReadings.clear();
    zReadings.clear();

    // Set the flag to indicate that calibration is complete
    isCalibrating = false;

    qDebug() << "Calibration complete. Bias values - X:" << xBias
             << " Y:" << yBias
             << " Z:" << zBias;
}
