set(SRC
	assembler.cpp
	cleaner.cpp
	removeexactduplicate.cpp
)

add_module_library(geometry-filter ${SRC})
target_link_libraries(geometry-filter PUBLIC geometry)
add_coverage(geometry-filter)
