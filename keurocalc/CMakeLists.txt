project(keurocalc VERSION "1.3.0")

########### next target ###############

set(keurocalc_SRCS
   ../currencies/currencies.cpp
   keurocalc.cpp
   main.cpp
   preferences.cpp )

qt5_add_resources(keurocalc_SRCS keurocalc.qrc)

ki18n_wrap_ui(keurocalc_SRCS
   calculator.ui
   settingsdialog.ui)

add_executable(keurocalc ${keurocalc_SRCS})

target_link_libraries(keurocalc
    Qt5::Widgets
    KF5::ConfigCore
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::I18n
    KF5::KIOCore
    KF5::XmlGui
    KF5::WidgetsAddons
)

install(TARGETS keurocalc ${INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(PROGRAMS org.kde.keurocalc.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES org.kde.keurocalc.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

install(FILES splash.png ../currencies/currencies.xml DESTINATION ${DATA_INSTALL_DIR}/keurocalc)

ecm_install_icons(ICONS 16-apps-keurocalc.png 32-apps-keurocalc.png 48-apps-keurocalc.png
	          DESTINATION ${ICON_INSTALL_DIR})
