qt5_wrap_ui(UIC_HEADERS
	group.ui
	list.ui
	settings.ui
)

add_custom_target(uic-settings DEPENDS ${UIC_HEADERS})
