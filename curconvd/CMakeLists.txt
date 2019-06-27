project(curconvd VERSION "1.3.0")

########### next target ###############

set(curconvd_SRCS
   ../currencies/currencies.cpp
   converter.cpp
   table.cpp
   main.cpp )

add_executable(curconvd ${curconvd_SRCS})

target_link_libraries(curconvd
    Qt5::Core
    Qt5::Xml
    Qt5::DBus
    KF5::I18n
    KF5::KIOCore
)

install(TARGETS curconvd ${INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(FILES ../currencies/currencies.xml DESTINATION ${DATA_INSTALL_DIR}/curconvd)

configure_file(org.kde.curconvd.service.in ${CMAKE_CURRENT_BINARY_DIR}/org.kde.curconvd.service)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.curconvd.service DESTINATION ${DBUS_SERVICES_INSTALL_DIR})
