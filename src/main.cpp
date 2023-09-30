#include <QGuiApplication>
#include <QScreen>
#include <asteroidapp.h>
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QDir>

#include "Settings.h"
#include "SdlGameController.h"
#include "RomManager.h"

void setupPaths() {
    if (QFile::exists(QDir::homePath() + "/.config/gpsp/gpsp.conf")) return;

    qDebug() << "Config file not found, setting up gpsp-menu.";
    qDebug() << "Config path " << QDir::homePath() + "/.config/gpsp";
    QDir().mkpath(QDir::homePath() + "/.config");
    QDir().mkpath(QDir::homePath() + "/.config/gpsp");
    QDir().mkpath(QDir::homePath() + "/roms");

    QSettings settings(QDir::homePath() + "/.config/gpsp/gpsp.conf", QSettings::IniFormat);
    settings.setValue("GAME_PATH", "");
    settings.setValue("GBA_BIOS", "/usr/share/gpsp/gba_bios.bin");
    settings.setValue("KEY_MAP", "1, 2, 3, 4, 5, 6");
    settings.setValue("ROM_PATH", QDir::homePath() + "/roms");
    settings.setValue("MAPPINGS_PATH", QDir::homePath() + "/.config/gpsp/gamecontrollerdb.txt");
    settings.sync();
    if(settings.status() == QSettings::NoError) {
        qDebug() << "Config file was properly setup.";
    } else {
        qDebug() << "Error while creating config file " << settings.status();
    }
}

int main(int argc, char *argv[])
{
    RomManager* romManager = RomManager::getInstance();

    setupPaths();

    Settings * settings = Settings::getInstance();
    settings->loadMappings();
    QString romPath = settings->getRomPath();
    qDebug() << romPath;

    romManager->setRoot(romPath);
    romManager->refresh();

    QScopedPointer<QGuiApplication> app(AsteroidApp::application(argc, argv));
    QScopedPointer<QQuickView> view(AsteroidApp::createView());

    qmlRegisterSingletonType<SdlGameController>("SdlGameController", 1, 0, "SdlGameController", &SdlGameController::qmlInstance);
    qmlRegisterSingletonType<RomManager>("RomManager", 1, 0, "RomManager", &RomManager::qmlInstance);
    view->setSource(QUrl("qrc:/qml/main.qml"));
    view->resize(app->primaryScreen()->size());
    view->show();
    return app->exec();
}
