set(SRC
	importer.cpp

	importer.h
)

add_library(importer-dxfplot ${SRC})
target_link_libraries(importer-dxfplot PRIVATE config)
