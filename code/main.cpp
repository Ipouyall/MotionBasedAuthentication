#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtSensors/QAccelerometer>
#include <QDebug>
#include "AccelerometerHandler.h"

int main(int argc, char *argv[])
{
    // QGuiApplication app(argc, argv);

    // QQmlApplicationEngine engine;

    // // Register the AccelerometerHandler
    // AccelerometerHandler accelerometerHandler;
    // engine.loadFromModule("FirstAndroidAccelometer", "Main");
    // // engine.load(QUrl::fromLocalFile("Main.qml"));

    // // engine.load(QUrl(QStringLiteral("file:///android_asset/Main.qml")));
    // // engine.load(QUrl(QStringLiteral("file:///") + QString::fromLatin1(__FILE__).replace("main.cpp", "Main.qml")));
    // if (engine.rootObjects().isEmpty())
    //     return -1;
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/FirstAndroidAccelometer/Main.qml"));
    AccelerometerHandler accelerometerHandler;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
#include "main.moc"
