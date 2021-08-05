set(SRC
	main.cpp
	FileItem.cpp
	FileModel.cpp
	RomManager.cpp
	SdlGameController.cpp
	Settings.cpp)
set(HEADERS
	FileItem.h
	FileModel.h
	RomManager.h
	SdlGameController.h
	Settings.h)

add_library(gpsp-menu ${SRC} ${HEADERS} resources.qrc)
set_target_properties(gpsp-menu PROPERTIES PREFIX "" SUFFIX "")

target_link_libraries(gpsp-menu PUBLIC
	AsteroidApp
	PkgConfig::SDL2)

install(TARGETS gpsp-menu
	DESTINATION ${CMAKE_INSTALL_BINDIR})
