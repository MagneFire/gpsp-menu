cmake_minimum_required(VERSION 3.6.0)

project(gpsp-menu
	VERSION 0.0.1
	DESCRIPTION "gpSP Menu for AsteroidOS")

find_package(ECM REQUIRED NO_MODULE)
find_package(AsteroidApp REQUIRED)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ASTEROID_MODULE_PATH})

include(FeatureSummary)
include(GNUInstallDirs)
include(ECMQMLModules)
include(AsteroidCMakeSettings)
include(AsteroidTranslations)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 sdl2 IMPORTED_TARGET REQUIRED)
set_property(GLOBAL APPEND PROPERTY _CMAKE_sdl2_TYPE REQUIRED)

if(SDL2_FOUND)
	set_property(GLOBAL APPEND PROPERTY PACKAGES_FOUND sdl2)
else()
	set_property(GLOBAL APPEND PROPERTY PACKAGES_NOT_FOUND sdl2)
endif()

add_subdirectory(src)

build_translations(i18n)
generate_desktop(${CMAKE_SOURCE_DIR} gpsp-menu)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
