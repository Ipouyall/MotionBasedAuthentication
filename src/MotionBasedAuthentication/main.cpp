#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "motionbasedauth.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<MotionBasedAuth> ("MotionBasedAuthentication", 1, 0, "MotionBasedAuthentication");

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
