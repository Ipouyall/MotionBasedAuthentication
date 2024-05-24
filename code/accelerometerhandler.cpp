#include "AccelerometerHandler.h"
#include <QDebug>
#include <QtGlobal>

AccelerometerHandler::AccelerometerHandler()
    : xBias(0.0), yBias(0.0), zBias(0.0), isCalibrating(true),
    velocityX(0.0), velocityY(0.0), velocityZ(0.0),
    distanceX(0.0), distanceY(0.0), distanceZ(0.0),
    intervalDistanceX(0.0), intervalDistanceY(0.0), intervalDistanceZ(0.0),
    wasMoving(false),zeroAccelartionCounter(0),
    kalmanX(0.0, 0.1, 0.1, 1), kalmanY(0.0, 0.1, 0.1, 1), kalmanZ(0.0, 0.1, 0.1, 1), startX(0.0), startY(0.0), startZ(0.0)
{
    // Create an accelerometer sensor
    accelerometer = new QAccelerometer(this);
    accelerometer->setAccelerationMode(QAccelerometer::User);
    accelerometer->start();

    // Create a timer for periodic reading
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &AccelerometerHandler::readSensorValues);
    timer->start(50); // 50 milliseconds interval

    // Set up a timer for calibration
    QTimer::singleShot(4000, this, &AccelerometerHandler::calculateBias); // 4 seconds calibration period

    // Start the elapsed timer
    gyroscopeHandler = new GyroscopeHandler();
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


            // adjustedX = kalmanX.update(adjustedX);
            // adjustedY = kalmanY.update(adjustedY);
            // adjustedZ = kalmanZ.update(adjustedZ);

            // // Apply threshold
            // if (qAbs(adjustedX) < threshold) adjustedX = 0;
            // if (qAbs(adjustedY) < threshold) adjustedY = 0;
            // if (qAbs(adjustedZ) < threshold) adjustedZ = 0;


            // Calculate time interval
            double deltaTime = elapsedTimer.elapsed() / 1000.0; // convert to seconds

            // Restart the elapsed timer
            elapsedTimer.restart();

            // qDebug() << "delta time " << deltaTime;

            // Update velocity
            velocityX += adjustedX * deltaTime;
            velocityY += adjustedY * deltaTime;
            velocityZ += adjustedZ * deltaTime;

            // qDebug() << "Velocity X: " << velocityX << " Velocity Y: " << velocityY << " Velocity Z: " << velocityZ;
            // qDebug() << "acc X: " << adjustedX << " acc Y: " << adjustedY << " acc Z: " << adjustedZ;
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
                // qDebug() << "=========================================================================================";
                if (zeroAccelartionCounter > 1) {
                    velocityX =0;
                    velocityY =0;
                    velocityZ =0;
                    zeroAccelartionCounter = 0;
                } else {
                    zeroAccelartionCounter++;
                }

            }

            // if (qAbs(velocityX) < velocityThreshold && qAbs(velocityY) < velocityThreshold && qAbs(velocityZ) < velocityThreshold) {
            //         qDebug() << "Distance between stops - X:" << intervalDistanceX
            //                  << " Y:" << intervalDistanceY
            //                  << " Z:" << intervalDistanceZ;
            //         intervalDistanceX = 0.0;
            //         intervalDistanceY = 0.0;
            //         intervalDistanceZ = 0.0;
            // }

            // Check if velocity is below velocityThreshold
            if (qAbs(velocityX) < velocityThreshold && qAbs(velocityY) < velocityThreshold && qAbs(velocityZ) < velocityThreshold) {
                if (wasMoving) {
                    // Save the current path segment
                    // paths.append(Path(startX, startY, startZ, intervalDistanceX, intervalDistanceY, intervalDistanceZ, gyroscopeHandler->getAngleZ()));
                    addNewPath(intervalDistanceX, intervalDistanceY, intervalDistanceZ, gyroscopeHandler->getAngleZ());
                    qDebug() << paths.last().toString();
                    printAllPaths();

                    // Update the start point for the next segment
                    // startX = intervalDistanceX;
                    // startY = intervalDistanceY;
                    startZ = intervalDistanceZ;

                    qDebug() << "Distance between stops - X:" << intervalDistanceX
                             << " Y:" << intervalDistanceY
                             << " Z:" << intervalDistanceZ;


                    // Reset interval distance
                    intervalDistanceX = 0.0;
                    intervalDistanceY = 0.0;
                    intervalDistanceZ = 0.0;

                    wasMoving = false;
                }
            } else {
                wasMoving = true;
            }

            // qDebug() << "Calibrated Accelerometer values - X:" << adjustedX
            //          << " Y:" << adjustedY
            //          << " Z:" << adjustedZ;
            // qDebug() << "Distance - X:" << distanceX
            //          << " Y:" << distanceY
            //          << " Z:" << distanceZ;
        }
    }
}


void AccelerometerHandler::addNewPath(double deltaX, double deltaY, double deltaZ, double angleZ)
{
    // double angleZ = gyroscopeHandler->getCurrentAngleZ();

    // Adjust the coordinates based on the rotation angle
    double  adjustedEndX, adjustedEndY;

    // QString direction;
    // if (angleZ >= -45 && angleZ < 45) {
    //     direction = "North";
    // } else if (angleZ >= 45 && angleZ < 135) {
    //     // 90 degrees rotation
    //     adjustedStartX = startY;
    //     adjustedStartY = -startX;
    //     adjustedEndX = endY;
    //     adjustedEndY = -endX;
    //     direction = "East";
    // } else if (angleZ >= 135 || angleZ < -135) {
    //     // 180 degrees rotation
    //     adjustedStartX = -startX;
    //     adjustedStartY = -startY;
    //     adjustedEndX = -endX;
    //     adjustedEndY = -endY;
    //     direction = "South";
    // } else if (angleZ >= -135 && angleZ < -45) {
    //     // 270 degrees rotation
    //     adjustedStartX = -startY;
    //     adjustedStartY = startX;
    //     adjustedEndX = -endY;
    //     adjustedEndY = endX;
    //     direction = "West";
    // }
    QString direction;
    if (angleZ >= -45 && angleZ < 45) {
        // North or South
        if (qAbs(deltaX) > qAbs(deltaY)) {
            direction = (deltaX > 0) ? "East" : "West";
            adjustedEndX = deltaX + startX;
            adjustedEndY = startY;

        } else {
            direction = (deltaY < 0) ? "South" : "North";
            adjustedEndX = startX;
            adjustedEndY = deltaY + startY;
        }
    } else if (angleZ >= 45 && angleZ < 135) {
        // East or West
        if (qAbs(deltaX) > qAbs(deltaY)) {
            direction = (deltaX < 0) ? "South" : "North";
            adjustedEndX = startX;
            adjustedEndY = deltaX + startY;
        } else {
            direction = (deltaY < 0) ? "East" : "West";// west left, Esat right
            adjustedEndX = (-deltaY) + startX;
            adjustedEndY = startY;
        }
    } else if (angleZ >= 135 || angleZ < -135) {
        // North or South
        if (qAbs(deltaX) > qAbs(deltaY)) {
            direction = (deltaX < 0) ? "East" : "West";
            adjustedEndX = (-deltaX) + startX;
            adjustedEndY = startY;
        } else {
            direction = (0 < deltaY) ? "South" : "North";
            adjustedEndX = startX;
            adjustedEndY = (-deltaY) + startY;
        }
    } else {// angleZ >= -135 && angleZ < -45
        // East or West
        if (qAbs(deltaX) > qAbs(deltaY)) {
            direction = (0 < deltaX) ? "South" : "North";
            adjustedEndX = startX;
            adjustedEndY = (-deltaX) + startY;
        } else {
            direction = (0 < deltaY) ? "East" : "West";
            adjustedEndX = deltaY + startX;
            adjustedEndY = startY;
        }
    }

    // paths.emplace_back(adjustedStartX, adjustedStartY, startZ, adjustedEndX, adjustedEndY, endZ, angleZ, direction);
    paths.append(Path(startX, startY, startZ, adjustedEndX, adjustedEndY, deltaZ, angleZ, direction));
    startX = adjustedEndX;
    startY = adjustedEndY;
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


void AccelerometerHandler::printAllPaths() const
{
    qDebug() << "All Paths:";
    for (const Path& path : paths) {
        qDebug() << path.toString();
    }
}
