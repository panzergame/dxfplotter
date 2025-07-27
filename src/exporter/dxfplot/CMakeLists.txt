set(SRC
	exporter.cpp

	exporter.h
)

add_library(exporter-dxfplot ${SRC})
target_link_libraries(exporter-dxfplot PRIVATE config)
add_coverage(exporter-dxfplot)
