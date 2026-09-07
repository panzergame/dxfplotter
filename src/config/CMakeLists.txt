set(MODULE_SRC
	node.cpp
	group.cpp
	list.cpp
	utils.cpp
	property.cpp
	${CMAKE_CURRENT_BINARY_DIR}/configgen.cpp
)

# config.cpp is a module implementation unit of config.config, not an interface unit.
add_module_library(config ${MODULE_SRC} NON_MODULE_SOURCES config.cpp)
add_dependencies(config generate-config)
target_link_libraries(config PUBLIC yaml-cpp::yaml-cpp common geometry)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/configgen.cpp
	COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/config_gen.py ${TEMPLATE_DIR}/config.xml ${CMAKE_CURRENT_SOURCE_DIR} configgen.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/configgen.cpp
	DEPENDS configgen.cpp.in ${TEMPLATE_DIR}/config.xml config_gen.py
)

add_custom_target(generate-config DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/configgen.cpp)
