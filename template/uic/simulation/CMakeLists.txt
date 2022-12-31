qt5_wrap_ui(UIC_HEADERS
	simulation.ui
)

add_custom_target(uic-simulation DEPENDS ${UIC_HEADERS})
