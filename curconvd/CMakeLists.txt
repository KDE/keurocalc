project(curconvd)

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
    KF5::I18n
    KF5::KIOCore
)

install(TARGETS curconvd ${INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(FILES ../currencies/currencies.xml DESTINATION ${DATA_INSTALL_DIR}/curconvd)
