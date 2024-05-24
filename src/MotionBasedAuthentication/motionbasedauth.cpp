#include "motionbasedauth.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QDebug>
class Path
{
public:
    Path(double startX, double startY, double startZ,
         double endX, double endY, double endZ, double angle, QString direction)
        : startX(startX), startY(startY), startZ(startZ),
        endX(endX), endY(endY), endZ(endZ), angle(angle), direction(direction) {}

    double getStartX() const { return startX; }
    double getStartY() const { return startY; }
    double getStartZ() const { return startZ; }
    double getEndX() const { return endX; }
    double getEndY() const { return endY; }
    double getEndZ() const { return endZ; }
    double getAngle() const { return angle; }
    QString getDirection() const { return direction; }

    QString toString() const {
        return QString("Path(%1, %2, %3, %4, %5, %6, %7, %8)")
            .arg(startX).arg(startY).arg(startZ)
            .arg(endX).arg(endY).arg(endZ)
            .arg(angle).arg(direction);
    }

    QJsonObject toJson() const {
        QJsonObject startObject;
        startObject["x"] = startX;
        startObject["y"] = startY;
        startObject["z"] = startZ;

        QJsonObject endObject;
        endObject["x"] = endX;
        endObject["y"] = endY;
        endObject["z"] = endZ;

        QJsonObject jsonObject;
        jsonObject["start"] = startObject;
        jsonObject["end"] = endObject;
        jsonObject["angle"] = angle;
        jsonObject["direction"] = direction;
        return jsonObject;
    }

private:
    double startX, startY, startZ;
    double endX, endY, endZ;
    double angle;
    QString direction;
};


MotionBasedAuth::MotionBasedAuth(QObject *parent) : QObject(parent) {}

QJsonArray MotionBasedAuth::getPath() const {
    return pathArray;
}

void MotionBasedAuth::startRecording() {
    // TODO: Add start record functionality/UI here
}

void MotionBasedAuth::endRecording() {
    // TODO: Add end record functionality/UI here
}

void MotionBasedAuth::authenticate() {
    // TODO: Fix codes here
    // TODO: now we just imagine that the user is authenticated successfully

    is_authenticated = true;
    emit statusChanged(is_authenticated);
}

void MotionBasedAuth::startPattern(){}
void MotionBasedAuth::endPattern(){}


void MotionBasedAuth::showData() {
    data = "A sample data";
    emit dataChanged(data);
}

void MotionBasedAuth::getPath() {
    // Simulate loading path data from JSON object
    Path path1(0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 45.0, "North");
    Path path2(1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 90.0, "East");
    Path path3(2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 135.0, "South");

    // Convert Path objects to QJsonObjects and add them to a QJsonArray
    QJsonArray jsonArray;
    jsonArray.append(path1.toJson());
    jsonArray.append(path2.toJson());
    jsonArray.append(path3.toJson());

    // Convert the QJsonArray to a QJsonDocument
    QJsonDocument jsonArrayDoc(jsonArray);

    QJsonArray pathArray = QJsonArray::fromStringList({
        "{\"start\": {\"x\": 0, \"y\": 0}, \"end\": {\"x\": 100, \"y\": 0}, \"direction\": \"top\", \"angle\": 0}",
        "{\"start\": {\"x\": 100, \"y\": 0}, \"end\": {\"x\": 100, \"y\": 100}, \"direction\": \"right\", \"angle\": 90}",
        "{\"start\": {\"x\": 100, \"y\": 100}, \"end\": {\"x\": 0, \"y\": 100}, \"direction\": \"bottom\", \"angle\": 180}",
        "{\"start\": {\"x\": 0, \"y\": 100}, \"end\": {\"x\": 0, \"y\": 0}, \"direction\": \"left\", \"angle\": -90}"
    });

    this->pathArray = jsonArray;
    emit pathChanged(pathArray);
}
