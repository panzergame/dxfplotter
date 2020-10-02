set(SRC
	exporter.cpp
	postprocessor.cpp
	pathpostprocessor.cpp

	exporter.h
	postprocessor.h
	pathpostprocessor.h
)

add_library(gcode ${SRC})
add_dependencies(gcode generate_config)
