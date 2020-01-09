set(SRC
	application.cpp
	assembler.cpp
	bulge.cpp
	path.cpp
	polyline.cpp
	task.cpp

	application.h
	assembler.h
	bulge.h
	constants.h
	path.h
	polyline.h
	task.h
)

add_library(core ${SRC})
