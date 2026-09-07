set(SRC
	basicpathitem.cpp
	bulgepainter.cpp
	offsettedpolylinepathitem.cpp
	pointpathitem.cpp
	polylinepathitem.cpp
	rubberband.cpp
	viewport.cpp
)

add_module_library(view-view2d ${SRC})
target_link_libraries(view-view2d PUBLIC model model-application PRIVATE config geometry)
add_dependencies(view-view2d uic)
