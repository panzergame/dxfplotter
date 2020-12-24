set(SRC
	bulgepainter.cpp
	offsetedpathitem.cpp
	pathitem.cpp
	rubberband.cpp
	viewport.cpp

	bulgepainter.h
	offsetedpathitem.h
	pathitem.h
	rubberband.h
	viewport.h
)

add_library(view-view2d ${SRC})
add_dependencies(view-view2d generate_config uic)
