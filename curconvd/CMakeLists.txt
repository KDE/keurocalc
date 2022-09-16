project(curconvd VERSION "1.3.1")

########### next target ###############

set(curconvd_SRCS
   ../currencies/currencies.cpp
   converter.cpp
   table.cpp
   ../currencies/currencies.h
   converter.h
   table.h
   main.cpp )

add_executable(curconvd ${curconvd_SRCS})

target_link_libraries(curconvd
    Qt::Core
    Qt::Xml
    Qt::DBus
    KF5::I18n
    KF5::KIOCore
)

install(TARGETS curconvd ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(FILES ../currencies/currencies.xml DESTINATION ${KDE_INSTALL_DATADIR}/curconvd)

configure_file(org.kde.curconvd.service.in ${CMAKE_CURRENT_BINARY_DIR}/org.kde.curconvd.service)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.curconvd.service DESTINATION ${KDE_INSTALL_DBUSSERVICEDIR})
