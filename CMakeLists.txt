cmake_minimum_required(VERSION 3.16)
project(keuromain)

set(QT_MIN_VERSION "5.15.2")
set(KF_MIN_VERSION "5.90.0")
set(KDE_COMPILERSETTINGS_LEVEL "5.82.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(ECMInstallIcons)
include(ECMDeprecationSettings)
set(QT_REQUIRED_VERSION "6.5.0")
set(KF_MIN_VERSION "5.240.0")
set(KF_MAJOR_VERSION "6")
find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Widgets
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
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

ecm_set_disabled_deprecation_versions(QT 6.4  KF 5.103.0)

add_subdirectory( keurocalc )
add_subdirectory( doc )
add_subdirectory( curconvd )
ki18n_install(po)
