set(SRC
	utils.cpp
	layer.cpp
	entityimporter.cpp
	importer.cpp
	interface.cpp
)

add_module_library(importer-dxf ${SRC} NON_MODULE_SOURCES importerimpl.cpp)
target_link_libraries(importer-dxf PUBLIC common geometry model)
