set(SRC
	bulgepainter.cpp
	info.cpp
	mainwindow.cpp
	offsetedpathitem.cpp
	path.cpp
	pathitem.cpp
	rubberband.cpp
	settings.cpp
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
	settings.h
	settingentry.h
	task.h
	tasklistmodel.h
	viewport.h
)

add_library(view ${SRC})
