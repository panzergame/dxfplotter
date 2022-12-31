set(SRC
	passesiterator.cpp
	renderer.cpp

	passesiterator.h
	renderer.h
)

add_library(exporter-renderer ${SRC})
add_dependencies(exporter-renderer generate-config)
add_coverage(exporter-renderer)
