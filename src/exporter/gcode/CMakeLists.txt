set(SRC
	exporter.cpp
	metadata.cpp
	postprocessor.cpp

	exporter.h
	metadata.h
	postprocessor.h
)

add_library(exporter-gcode ${SRC})
target_link_libraries(exporter-gcode PRIVATE config)
add_coverage(exporter-gcode)
