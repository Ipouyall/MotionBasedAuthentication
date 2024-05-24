#include "motionbasedauth.h"

MotionBasedAuth::MotionBasedAuth(QObject *parent) :
    QObject(parent) {
    sensorHandler = new SensorHandler();
    is_auth_pattern = true;
    attempt_number = 1;
    connect(sensorHandler, &SensorHandler::newPathAdded, this, &MotionBasedAuth::newPathReceived);
}

QJsonArray MotionBasedAuth::getPath() const {
    return pathArray;
}

void MotionBasedAuth::startRecording() {
    // TODO: Add start record functionality/UI here
    sensorHandler->reset();
    sensorHandler->start();
    is_auth_pattern = false;
}

void MotionBasedAuth::endRecording() {
    sensorHandler->stop();
    if (attemptPaths.size() > 0)
        attemptPaths.last() = sensorHandler->getPaths();
    else
        attemptPaths.append(sensorHandler->getPaths());
    qDebug() << "attempt Path: ";
    sensorHandler->printAllPaths();
    attempt_number++;
    authenticate();
}

void MotionBasedAuth::authenticate() {
    // TODO: Fix codes here
    // TODO: now we just imagine that the user is authenticated successfully
    QJsonObject result = this->comparePaths(attemptPaths.last());
    is_authenticated = result["authenticated"].toBool();
    if (!is_authenticated) {
        emit logChanged(result["error"].toString());
    }
    emit statusChanged(is_authenticated);
}

void MotionBasedAuth::startPattern(){
    is_auth_pattern = true;
    sensorHandler->reset();
    sensorHandler->start();
}
void MotionBasedAuth::endPattern(){
    sensorHandler->stop();
    authPaths = sensorHandler->getPaths();
    qDebug() << "auth Path: ";
    sensorHandler->printAllPaths();
}

void MotionBasedAuth::reset() {
    is_auth_pattern = true;
    attempt_number = 1;
    sensorHandler->reset();
    authPaths.clear();
    attemptPaths.clear();
    qDebug() << "MotionBasedAuth reset. auth path size: " << authPaths.size() << ", attemptSize: " << attemptPaths.size();
}


void MotionBasedAuth::showData() {
    data = this->formatData();
    emit dataChanged(data);
}

QString MotionBasedAuth::formatData() {
    QString result = "================================\n";
    result += "Authentication Data:\n";
    for (const Path &path : authPaths) {
        result += formatPath(path);
    }
    result += "================================\n";

    for (int i = 0; i < attemptPaths.size(); ++i) {
        result += QString("Attempt %1:\n").arg(i + 1);
        for (const Path &path : attemptPaths[i]) {
            result += formatPath(path);
        }
        result += "================================\n";
    }
    return result;
}

QString MotionBasedAuth::formatPath(const Path &path) {
    return QString("Start:(X: %1, Y: %2, Z: %3)\n"
                   "End:(X: %4, Y: %5, Z: %6)\n"
                   "Angle: %7\n"
                   "Direction: %8\n\n")
        .arg(QString::number(path.getStartX(), 'f', 3))
        .arg(QString::number(path.getStartY(), 'f', 3))
        .arg(QString::number(path.getStartZ(), 'f', 3))
        .arg(QString::number(path.getEndX(), 'f', 3))
        .arg(QString::number(path.getEndY(), 'f', 3))
        .arg(QString::number(path.getEndZ(), 'f', 3))
        .arg(path.getAngle())
        .arg(path.getDirection());
}

void MotionBasedAuth::newPathReceived(QVector<Path> newPath) {
    if (is_auth_pattern) {
        authPaths = sensorHandler->getPaths();
    } else {
        if (attempt_number > attemptPaths.size()) {
            attemptPaths.append(sensorHandler->getPaths());
        } else {
            attemptPaths.last() = sensorHandler->getPaths();
        }
    }
    this->showData();
    this->getPath();
}

void MotionBasedAuth::getPath() {
    QJsonArray pathArray = QJsonArray();
    QVector<Path> paths = sensorHandler->getPaths();
    for (int i = 0; i < paths.size(); ++i) {
        pathArray.append(paths[i].toJson());
    }
    this->pathArray = pathArray;
    emit pathChanged(pathArray);
}

QJsonObject MotionBasedAuth::comparePaths(const QVector<Path>& attemptPath) {
    QJsonObject result;

    // Check if the number of elements in both vectors is the same
    if (authPaths.size() != attemptPath.size()) {
        result["error"] = "Different number of paths";
        result["authenticated"] = false;
        return result;
    }

    // Iterate over each pair of paths and compare them
    for (int i = 0; i < authPaths.size(); ++i) {
        const Path& auth = authPaths.at(i);
        const Path& attempt = attemptPath.at(i);

        // Compare start points with a 50% error for each coordinate
        if (std::fabs(auth.getStartX() - attempt.getStartX()) > 0.75 * std::fabs(auth.getStartX()) ||
            std::fabs(auth.getStartY() - attempt.getStartY()) > 0.75 * std::fabs(auth.getStartY())) {
            result["error"] = "Mismatch in start point coordinates";
            result["authenticated"] = false;
            return result;
        }

        // Compare end points with a 50% error for each coordinate
        if (std::fabs(auth.getEndX() - attempt.getEndX()) > 0.75 * std::fabs(auth.getEndX()) ||
            std::fabs(auth.getEndY() - attempt.getEndY()) > 0.75 * std::fabs(auth.getEndY())) {
            result["error"] = "Mismatch in end point coordinates";
            result["authenticated"] = false;
            return result;
        }

        // Compare other attributes (e.g., angle, direction) if needed
        // For simplicity, assume that angle and direction are equal for authentication and attempt paths
        if (auth.getAngle() != attempt.getAngle()) {
            result["error"] = "Mismatch in angle";
            result["authenticated"] = false;
            return result;
        }
        if (auth.getDirection() != attempt.getDirection()) {
            result["error"] = "Mismatch in direction";
            result["authenticated"] = false;
            return result;
        }
    }

    // If no mismatches were found, return an empty object
    result["authenticated"] = true;
    return result;
}
