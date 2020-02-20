add_subdirectory(spline)

set(SRC
	assembler.cpp

	assembler.h
	utils.h
)

add_library(geometry ${SRC})
