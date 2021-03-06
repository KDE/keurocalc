cmake_minimum_required(VERSION 3.0)
project(keuromain)

set(QT_MIN_VERSION "5.3.0")
set(KF5_MIN_VERSION "5.2.0")

find_package(ECM 1.7.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(ECMInstallIcons)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Widgets
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Config
    ConfigWidgets
    CoreAddons
    DocTools
    I18n
    KIO
    WidgetsAddons
    XmlGui
)

include(CheckIncludeFile)

check_include_file(xlocale.h HAVE_XLOCALE_H)

configure_file(config-keurocalc.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-keurocalc.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f00)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x054800)

add_subdirectory( keurocalc )
add_subdirectory( doc )
add_subdirectory( curconvd )
