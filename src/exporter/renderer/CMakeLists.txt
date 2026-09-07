set(SRC
	passesiterator.cpp
	renderer.cpp
)

add_module_library(exporter-renderer ${SRC})
target_link_libraries(exporter-renderer PUBLIC geometry model PRIVATE config)
add_coverage(exporter-renderer)
