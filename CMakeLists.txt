cmake_minimum_required(VERSION 2.8)
project(keuromain)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

# search packages used by KDE
find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include (MacroLibrary)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
link_directories (${KDE4_LIB_DIR})
include_directories ( ${QDBUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

add_subdirectory( keurocalc )
add_subdirectory( curconvd )
