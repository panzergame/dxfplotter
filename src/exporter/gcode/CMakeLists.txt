set(SRC
	exporter.cpp
	postprocessor.cpp
	pathpostprocessor.cpp

	exporter.h
	postprocessor.h
	pathpostprocessor.h
)

add_library(exporter-gcode ${SRC})
add_dependencies(exporter-gcode generate-config)
add_coverage(exporter-gcode)
