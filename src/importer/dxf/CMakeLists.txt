set(SRC
	entityimporter.cpp
	importer.cpp
	interface.cpp
	layer.cpp

	entityimporter.h
	importer.h
	interface.h
	utils.h
	layer.h
)

add_library(dxf ${SRC})
