add_subdirectory(settings)

qt5_wrap_ui(UIC_HEADERS
	transform.ui
	mirror.ui
	setorigin.ui
)

add_custom_target(uic-dialogs DEPENDS ${UIC_HEADERS})
add_dependencies(uic-dialogs uic-dialogs-settings)
