#ifndef PATH_H
#define PATH_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

class Path
{
public:
    Path(double startX, double startY, double startZ,
         double endX, double endY, double endZ, double angle, QString direction);

    double getStartX() const;
    double getStartY() const;
    double getStartZ() const;
    double getEndX() const;
    double getEndY() const;
    double getEndZ() const;
    double getAngle() const;
    QString getDirection() const;
    QString toString() const;
    QJsonObject toJson() const;

private:
    double startX, startY, startZ;
    double endX, endY, endZ;
    double angle;
    QString direction; // Direction attribute
};

#endif // PATH_H
