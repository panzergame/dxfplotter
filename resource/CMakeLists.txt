qt_add_resources(SOURCES resource.qrc)

add_library(resource ${SOURCES})

install(FILES dxfplotter.png DESTINATION share/icons/hicolor/256x256/apps/)
install(FILES dxfplotter.desktop DESTINATION share/applications/)
