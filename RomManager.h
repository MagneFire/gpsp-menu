#ifndef ROMMANAGER_H
#define ROMMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>

#include <QProcess>

#include "FileModel.h"

class RomManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(RomManager)
    Q_PROPERTY(QString root READ getRoot WRITE setRoot NOTIFY rootChanged)

    RomManager();
    ~RomManager();
public:
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
    static RomManager *getInstance();

    void setRoot(const QString path);
    const QString getRoot();

    void refresh();
    Q_INVOKABLE QObject* getModel();
    Q_INVOKABLE void run(const QString size);
signals:
    void rootChanged();
private:
    static RomManager * instance;

    QString root;
    FileModel model;
    QProcess process;
};

#endif