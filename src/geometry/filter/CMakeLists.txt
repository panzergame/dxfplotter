set(SRC
	assembler.cpp
	cleaner.cpp
	removeexactduplicate.cpp

	assembler.h
	cleaner.h
	removeexactduplicate.h
)

add_library(geometry-filter ${SRC})
add_coverage(geometry-filter)
