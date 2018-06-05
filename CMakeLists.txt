cmake_minimum_required(VERSION 3.0)
project(keuromain)

set(QT_MIN_VERSION "5.3.0")
set(KF5_MIN_VERSION "5.2.0")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(ECMInstallIcons)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${QT_INCLUDES} ${KF5_INCLUDES} ${QDBUS_INCLUDE_DIRS})
add_definitions(${KF5_DEFINITIONS} ${QT_DEFINITIONS})

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Widgets
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    I18n
    KIO
)

add_subdirectory( keurocalc )
add_subdirectory( curconvd )
