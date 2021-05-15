set(SRC
	exporter.cpp

	exporter.h
)

add_library(exporter-dxfplot ${SRC})
add_dependencies(exporter-dxfplot generate_config)
add_coverage(exporter-dxfplot)
