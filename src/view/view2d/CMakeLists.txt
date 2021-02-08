set(SRC
	basicpathitem.cpp
	bulgepainter.cpp
	offsetedpolylinepathitem.cpp
	polylinepathitem.cpp
	rubberband.cpp
	viewport.cpp

	basicpathitem.h
	bulgepainter.h
	offsetedpolylinepathitem.h
	polylinepathitem.h
	rubberband.h
	viewport.h
)

add_library(view-view2d ${SRC})
add_dependencies(view-view2d generate_config uic)
