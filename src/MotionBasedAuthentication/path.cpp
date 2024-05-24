#include "Path.h"

Path::Path(double startX, double startY, double startZ,
           double endX, double endY, double endZ, double angle, QString direction)
    : startX(startX), startY(startY), startZ(startZ),
    endX(endX), endY(endY), endZ(endZ), angle(angle), direction(direction)
{
}

double Path::getStartX() const { return startX; }
double Path::getStartY() const { return startY; }
double Path::getStartZ() const { return startZ; }
double Path::getEndX() const { return endX; }
double Path::getEndY() const { return endY; }
double Path::getEndZ() const { return endZ; }
double Path::getAngle() const {return angle; }

QString Path::toString() const {
    return QString("Path(Start: [%1, %2, %3], End: [%4, %5, %6], Angle: %7, Direction: %8)")
        .arg(startX).arg(startY).arg(startZ)
        .arg(endX).arg(endY).arg(endZ).arg(angle).arg(direction);
}
