add_subdirectory(settings)

set(SRC
	bulgepainter.cpp
	info.cpp
	mainwindow.cpp
	offsetedpathitem.cpp
	path.cpp
	pathitem.cpp
	rubberband.cpp
	task.cpp
	tasklistmodel.cpp
	viewport.cpp

	bulgepainter.h
	info.h
	mainwindow.h
	offsetedpathitem.h
	path.h
	pathitem.h
	rubberband.h
	task.h
	tasklistmodel.h
	viewport.h
)

add_library(view ${SRC})
add_dependencies(view generate_config uic)

