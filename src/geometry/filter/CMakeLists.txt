set(SRC
	assembler.cpp
	cleaner.cpp
	removeexactduplicate.cpp
	sorter.cpp

	assembler.h
	cleaner.h
	removeexactduplicate.h
	sorter.h
)

add_library(geometry-filter ${SRC})
add_coverage(geometry-filter)
