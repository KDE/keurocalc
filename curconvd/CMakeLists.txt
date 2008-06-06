project( curconvd )

########### next target ###############

set( curconvd_SRCS
   ../currencies/currencies.cpp
   converter.cpp
   table.cpp
   main.cpp )

kde4_add_executable(curconvd ${curconvd_SRCS} )

target_link_libraries(curconvd ${KDE4_KIO_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KDEPRINT_LIBS} )

install(TARGETS curconvd ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( FILES ../currencies/currencies.xml DESTINATION ${DATA_INSTALL_DIR}/curconvd )
