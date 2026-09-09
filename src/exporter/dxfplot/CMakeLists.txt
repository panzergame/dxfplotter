set(SRC
	exporter.cpp
)

add_module_library(exporter-dxfplot ${SRC})
target_link_libraries(exporter-dxfplot PUBLIC model PRIVATE config geometry serializer)
add_coverage(exporter-dxfplot)
