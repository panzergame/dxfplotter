set(SRC
	bulgepainter.cpp
	settings.cpp
	mainwindow.cpp
	offsetedpathitem.cpp
	path.cpp
	pathitem.cpp
	pathmenu.cpp
	rubberband.cpp
	task.cpp
	tasklistmodel.cpp
	viewport.cpp

	bulgepainter.h
	settings.h
	settingentry.h
	mainwindow.h
	offsetedpathitem.h
	path.h
	pathitem.h
	pathmenu.h
	rubberband.h
	task.h
	tasklistmodel.h
	viewport.h
)

add_library(view ${SRC})
add_dependencies(view generate_config)
