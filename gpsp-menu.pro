TARGET = gpsp-menu
CONFIG += asteroidapp

HEADERS +=     FileItem.h \
               FileModel.h
SOURCES +=     main.cpp \
               FileItem.cpp \
               FileModel.cpp
RESOURCES +=   resources.qrc
OTHER_FILES += main.qml

lupdate_only{ SOURCES += i18n/gpsp-menu.desktop.h }
TRANSLATIONS = $$files(i18n/$$TARGET.*.ts)
