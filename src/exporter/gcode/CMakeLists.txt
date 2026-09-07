set(SRC
	metadata.cpp
	exporter.cpp
	postprocessor.cpp
)

add_module_library(exporter-gcode ${SRC} NON_MODULE_SOURCES exporterimpl.cpp)
target_link_libraries(exporter-gcode PUBLIC common geometry model PRIVATE config exporter-renderer)
add_coverage(exporter-gcode)
