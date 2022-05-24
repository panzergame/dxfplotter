add_subdirectory(settings)

qt_wrap_ui(UIC_HEADERS
	transform.ui
	mirror.ui
)

add_custom_target(uic-dialogs DEPENDS ${UIC_HEADERS})
add_dependencies(uic-dialogs uic-dialogs-settings)
