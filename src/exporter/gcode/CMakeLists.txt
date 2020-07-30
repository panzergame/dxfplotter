set(SRC
	exporter.cpp
	format.cpp
	postprocessor.cpp
	pathpostprocessor.cpp

	exporter.h
	format.h
	postprocessor.h
	pathpostprocessor.h
)

add_library(gcode ${SRC})
