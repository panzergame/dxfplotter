set(SRC
	basicpathitem.cpp
	bulgepainter.cpp
	offsettedpolylinepathitem.cpp
	pointpathitem.cpp
	polylinepathitem.cpp
	rubberband.cpp
	viewport.cpp

	basicpathitem.h
	bulgepainter.h
	offsettedpolylinepathitem.h
	pointpathitem.h
	polylinepathitem.h
	rubberband.h
	viewport.h
)

add_library(view-view2d ${SRC})
target_link_libraries(view-view2d PRIVATE config)
add_dependencies(view-view2d uic)
