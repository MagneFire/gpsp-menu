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

    QString getMappingsPath() const;
    void setMappingsPath(QString path);
    void loadMappings();
    void saveMappings();
    void addMapping(QString guid, QString mapping);
private:
    Settings();
    virtual ~Settings() {};

    void saveSettings();
    static Settings* instance;
    QSettings * settings;
    QStringList mappings;
};

#endif