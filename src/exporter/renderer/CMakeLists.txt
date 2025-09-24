set(SRC
	passesiterator.cpp
	renderer.cpp

	passesiterator.h
	renderer.h
)

add_library(exporter-renderer ${SRC})
target_link_libraries(exporter-renderer PRIVATE config)
add_coverage(exporter-renderer)
