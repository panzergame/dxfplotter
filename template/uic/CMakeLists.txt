add_subdirectory(dialogs)
add_subdirectory(simulation)

qt_wrap_ui(UIC_HEADERS
	info.ui
	mainwindow.ui
	path.ui
	profile.ui
	task.ui
)

add_custom_target(uic DEPENDS ${UIC_HEADERS})
add_dependencies(uic uic-dialogs uic-simulation)

