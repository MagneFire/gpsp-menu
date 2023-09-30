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
    settings = new QSettings(QDir::homePath() + "/.config/gpsp/gpsp.conf", QSettings::IniFormat);
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

QString Settings::getMappingsPath() const {
    return settings->value("MAPPINGS_PATH").toString();
}

void Settings::setMappingsPath(QString path) {
    settings->setValue("MAPPINGS_PATH", path);
    qDebug() << settings->value("MAPPINGS_PATH").toString();
    saveSettings();
}

void Settings::loadMappings() {
    QFile mappingsFile(getMappingsPath());
    if (mappingsFile.open(QIODevice::ReadOnly | QFile::Text)) {
        QTextStream in(&mappingsFile);
        mappings.clear();
        while (!in.atEnd()) {
            QString line = in.readLine();
            mappings << line;
        }
        mappingsFile.close();
    }
}

void Settings::saveMappings() {
    QFile mappingsFile(getMappingsPath());
    if (mappingsFile.open(QIODevice::WriteOnly | QFile::Text)) {
        QTextStream stream(&mappingsFile);
        for ( const auto& mapping : mappings ) {
            stream << mapping << '\n';
        }
        mappingsFile.close();
    }
}

void Settings::addMapping(QString guid, QString mapping) {
    int index = mappings.indexOf(QRegExp("^" + guid + ".+"));
    if (index >= 0) {
        mappings[index] = mapping;
    } else {
        mappings.append(mapping);
    }
}