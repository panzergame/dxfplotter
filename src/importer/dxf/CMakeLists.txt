add_subdirectory(converter)

set(SRC
	importer.cpp
	interface.cpp

	importer.h
	interface.h
)

add_library(dxf ${SRC})
