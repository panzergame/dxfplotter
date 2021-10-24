add_subdirectory(dialogs)

qt5_wrap_ui(UIC_HEADERS
	info.ui
	mainwindow.ui
	path.ui
	profile.ui
	task.ui
)

add_custom_target(uic DEPENDS ${UIC_HEADERS})
add_dependencies(uic uic-dialogs)

