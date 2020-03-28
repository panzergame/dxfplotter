set(SRC
	settings.cpp
	mainwindow.cpp
	path.cpp
	pathitem.cpp
	rubberband.cpp
	task.cpp
	tasklistmodel.cpp
	viewport.cpp

	settings.h
	settingentry.h
	mainwindow.h
	path.h
	pathitem.h
	rubberband.h
	task.h
	tasklistmodel.h
	viewport.h
)

add_library(view ${SRC})
add_dependencies(view generate_config)
