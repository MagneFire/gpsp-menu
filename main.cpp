#include <QGuiApplication>
#include <QScreen>
#include <asteroidapp.h>
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QAbstractListModel>
#include <QDir>

#include "FileItem.h"
#include "FileModel.h"

int main(int argc, char *argv[])
{
    FileModel model;

    QDir directory("/home/ceres/roms");
    QStringList roms = directory.entryList(QStringList() << "*.gba" << "*.GBA",QDir::Files);
    foreach(QString rom, roms) {
        QFileInfo fileInfo(rom);
        model.addFile(FileItem(fileInfo.fileName(), rom));
    }   
   
    QScopedPointer<QGuiApplication> app(AsteroidApp::application(argc, argv));
    QScopedPointer<QQuickView> view(AsteroidApp::createView());

    view->rootContext()->setContextProperty("myModel", &model);
    view->setSource(QUrl("qrc:/main.qml"));
    view->resize(app->primaryScreen()->size());
    view->show();
    return app->exec();
}
