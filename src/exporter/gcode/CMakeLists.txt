set(SRC
	exporter.cpp
	format.cpp
	postprocessor.cpp

	exporter.h
	format.h
	postprocessor.h
)

add_library(gcode ${SRC})
add_dependencies(gcode generate_config)
