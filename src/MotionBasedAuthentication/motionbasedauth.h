#ifndef MOTIONBASEDAUTH_H
#define MOTIONBASEDAUTH_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class MotionBasedAuth : public QObject
{
    Q_OBJECT

public:
    explicit MotionBasedAuth(QObject *parent = nullptr);

    Q_PROPERTY(bool is_authenticated NOTIFY statusChanged FINAL)
    Q_PROPERTY(QString data NOTIFY dataChanged FINAL)
    Q_PROPERTY(QJsonArray  pathArray NOTIFY pathChanged FINAL)

    QJsonArray getPath() const;

Q_SIGNALS:
    void statusChanged(bool result);
    void dataChanged(QString data);
    void pathChanged(QJsonArray path);

public Q_SLOTS:
    void startRecording();
    void endRecording();
    void authenticate();
    void showData();
    void getPath();

private:
    bool is_authenticated;
    QString data;
    QJsonArray pathArray;
};

#endif // MOTIONBASEDAUTH_H