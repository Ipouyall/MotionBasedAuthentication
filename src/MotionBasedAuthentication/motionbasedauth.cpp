#include "motionbasedauth.h"

MotionBasedAuth::MotionBasedAuth(QObject *parent) :
    QObject(parent) {
    sensorHandler = new SensorHandler();
}

QJsonArray MotionBasedAuth::getPath() const {
    return pathArray;
}

void MotionBasedAuth::startRecording() {
    // TODO: Add start record functionality/UI here
    sensorHandler->reset();
    sensorHandler->start();
}

void MotionBasedAuth::endRecording() {
    sensorHandler->stop();
    attemptPaths.append(sensorHandler->getPaths());
    qDebug() << "attempt Path: ";
    sensorHandler->printAllPaths();
    authenticate();
}

void MotionBasedAuth::authenticate() {
    // TODO: Fix codes here
    // TODO: now we just imagine that the user is authenticated successfully

    is_authenticated = true;
    emit statusChanged(is_authenticated);
}

void MotionBasedAuth::startPattern(){
    sensorHandler->reset();
    sensorHandler->start();
}
void MotionBasedAuth::endPattern(){
    sensorHandler->stop();
    authPaths = sensorHandler->getPaths();
    qDebug() << "auth Path: ";
    sensorHandler->printAllPaths();
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
