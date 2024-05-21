#include "motionbasedauth.h"

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

void MotionBasedAuth::showData() {
    data = "A sample data";
    emit dataChanged(data);
}

void MotionBasedAuth::getPath() {
    // Simulate loading path data from JSON object
    QJsonArray pathArray = QJsonArray::fromStringList({
        "{\"start\": {\"x\": 0, \"y\": 0}, \"end\": {\"x\": 100, \"y\": 0}, \"direction\": \"top\", \"angle\": 0}",
        "{\"start\": {\"x\": 100, \"y\": 0}, \"end\": {\"x\": 100, \"y\": 100}, \"direction\": \"right\", \"angle\": 90}",
        "{\"start\": {\"x\": 100, \"y\": 100}, \"end\": {\"x\": 0, \"y\": 100}, \"direction\": \"bottom\", \"angle\": 180}",
        "{\"start\": {\"x\": 0, \"y\": 100}, \"end\": {\"x\": 0, \"y\": 0}, \"direction\": \"left\", \"angle\": -90}"
    });

    this->pathArray = pathArray;
    emit pathChanged(pathArray);
}
