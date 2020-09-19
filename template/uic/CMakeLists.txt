add_subdirectory(settings)

qt5_wrap_ui(UIC_HEADERS
	info.ui
	mainwindow.ui
	path.ui
	task.ui
)

add_custom_target(uic DEPENDS ${UIC_HEADERS})
