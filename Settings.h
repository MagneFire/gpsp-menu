#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings
{
public:
    static Settings * getInstance();

    QString getRomPath() const;
    void setRom(QString path);
private:
    Settings();
    virtual ~Settings() {};

    static Settings* instance;
    QSettings * settings;
};

#endif