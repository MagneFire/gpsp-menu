#include "Settings.h"
#include <QDebug>
#include <QDir>

Settings* Settings::instance;

Settings* Settings::getInstance() {
    if (instance == nullptr) {
        instance = new Settings();
    }
    return instance;
}

Settings::Settings() {
    settings = new QSettings(QDir::homePath() + "/.gpsp/gpsp.conf", QSettings::IniFormat);
    if(settings->status() == QSettings::NoError) {
        qDebug() << "Load successful";
    } else {
        qDebug() << "Load error" << settings->status();
    }
}

QString Settings::getRomPath() const {
    return settings->value("ROM_PATH").toString();
}

void Settings::setRom(QString path) {
    settings->setValue("GAME_PATH", path);
    qDebug() << settings->value("GAME_PATH").toString();
    settings->sync();
    if(settings->status() == QSettings::NoError) {
        qDebug() << "Save successful";
    } else {
        qDebug() << "Save error " << settings->status();
    }
}