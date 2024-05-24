#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "motionbasedauth.h"
#include "sensorhandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<MotionBasedAuth> ("MotionBasedAuthentication", 1, 0, "MotionBasedAuthentication");
    // SensorHandler sensor;
    // sensor.start();

    const QUrl url(QStringLiteral("qrc:/MotionBasedAuthentication/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
