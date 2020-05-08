#include "RomManager.h"
#include "FileItem.h"
#include <QAbstractListModel>
#include <QDir>
#include <QFileInfo>
#include "Settings.h"

RomManager * RomManager::instance;

RomManager::RomManager() {

}

RomManager::~RomManager() {

}
QObject * RomManager::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return getInstance();
}

RomManager * RomManager::getInstance() {
    if (instance == nullptr)
        instance = new RomManager();

    return instance;
}

void RomManager::setRoot(const QString path) {
    root = path;
    emit rootChanged();
}

const QString RomManager::getRoot() {
    return root;
}

void RomManager::refresh() {
    QDir directory(root);
    QFileInfoList roms = directory.entryInfoList(QStringList() << "*.gba" << "*.GBA" << "*.gbc" << "*.GBC" << "*.gb" << "*.GB",QDir::Files);
    foreach(QFileInfo rom, roms) {
        model.addFile(FileItem(rom.fileName(), rom.absoluteFilePath()));
    }
}

QObject* RomManager::getModel() {
    return &model;
}

void RomManager::run(const QString path) {
    Settings * settings = Settings::getInstance();
    settings->setRom(path);
    QFileInfo file(path);
    QString rom_ext = file.suffix().toLower();
    if (rom_ext.contains("gba")) {
        process.start("systemctl", QStringList() << "--user" << "start" << "gpsp");
    } else {
        process.start("systemctl", QStringList() << "--user" << "start" << "gearboy");
    }
}

void RomManager::run(int index) {
    QModelIndex modelIndex = model.index(index, 0, QModelIndex());
    QVariant path = model.data(modelIndex, FileModel::PathRole);
    run(path.toString());
}
