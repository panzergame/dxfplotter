set(SRC
	importer.cpp

	importer.h
)

add_library(importer-dxfplot ${SRC})
add_dependencies(importer-dxfplot generate-config)
