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
void Settings::saveSettings() {
    settings->sync();
    if(settings->status() == QSettings::NoError) {
        qDebug() << "Save successful";
    } else {
        qDebug() << "Save error " << settings->status();
    }
}

QString Settings::getRomPath() const {
    return settings->value("ROM_PATH").toString();
}

void Settings::setRom(QString path) {
    settings->setValue("GAME_PATH", path);
    qDebug() << settings->value("GAME_PATH").toString();
    saveSettings();
}

void Settings::setActiveGameController(int joyId) {
    settings->setValue("JOYSTICK_ACTIVE", joyId);
    qDebug() << settings->value("JOYSTICK_ACTIVE").toString();
    saveSettings();
}
void Settings::setGameControllerMapping(int joyId, QString mapping) {
    settings->setValue("JOYSTICK_MAP" + QString::number(joyId), mapping);
    qDebug() << settings->value("JOYSTICK_MAP" + QString::number(joyId)).toString();
    saveSettings();
}
QString Settings::getKeyMapping(int joyId) {
    return settings->value("JOYSTICK_MAP" + QString::number(joyId)).toString();
}