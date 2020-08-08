set(CONFIG_RAW_XML configrawxml.cpp)

set(SRC
	config.cpp
	group.cpp
	node.cpp
	section.cpp
	${CONFIG_RAW_XML}
	variable.cpp

	config.h
	group.h
	node.h
	section.h
	variable.h
)

add_custom_command(
       OUTPUT ${CONFIG_RAW_XML}
       COMMAND ${CMAKE_COMMAND} -DINPUT=${TEMPLATE_DIR}/config.xml -DTEMPLATE=${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_RAW_XML}.in -DOUTPUT=${CONFIG_RAW_XML} -P ${CMAKE_CURRENT_SOURCE_DIR}/generaterawxml.cmake
       DEPENDS ${TEMPLATE_DIR}/config.xml ${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_RAW_XML}.in 
)

add_library(config ${SRC})
