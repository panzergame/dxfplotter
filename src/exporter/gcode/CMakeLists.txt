set(SRC
	exporter.cpp
	metadata.cpp
	postprocessor.cpp

	exporter.h
	metadata.h
	postprocessor.h
)

add_library(exporter-gcode ${SRC})
add_dependencies(exporter-gcode generate-config)
add_coverage(exporter-gcode)
