TARGET = gpsp-menu
CONFIG += link_pkgconfig asteroidapp

PKGCONFIG += sdl2

HEADERS +=     FileItem.h \
               FileModel.h \
               Settings.h \
               RomManager.h \
               SdlGameController.h
SOURCES +=     main.cpp \
               FileItem.cpp \
               FileModel.cpp \
               Settings.cpp \
               RomManager.cpp \
               SdlGameController.cpp
RESOURCES +=   resources.qrc
OTHER_FILES += main.qml

lupdate_only{ SOURCES += i18n/gpsp-menu.desktop.h }
TRANSLATIONS = $$files(i18n/$$TARGET.*.ts)
