set(SRC
	importer.cpp
)

add_module_library(importer-dxfplot ${SRC})
target_link_libraries(importer-dxfplot PUBLIC model PRIVATE config common geometry serializer)
