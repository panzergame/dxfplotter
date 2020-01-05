set(SRC
	assembler.cpp
	bulge.cpp
	path.cpp
	polyline.cpp

	assembler.h
	bulge.h
	constants.h
	path.h
	polyline.h
)

add_library(core ${SRC})
