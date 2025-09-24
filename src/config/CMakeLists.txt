set_property(SOURCE config.h PROPERTY SKIP_AUTOGEN ON)

set(SRC
	config.cpp
	node.cpp

	config.h
	group.h
	list.h
	node.h
	property.h
)

add_library(config ${SRC})
add_dependencies(config generate-config)
target_link_libraries(config PUBLIC yaml-cpp::yaml-cpp)

add_custom_command(
	OUTPUT config.h
	COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/config_gen.py ${TEMPLATE_DIR}/config.xml ${CMAKE_CURRENT_SOURCE_DIR} config.h.in config.h
	DEPENDS config.h.in ${TEMPLATE_DIR}/config.xml config_gen.py
)

add_custom_target(generate-config DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/config.h)
