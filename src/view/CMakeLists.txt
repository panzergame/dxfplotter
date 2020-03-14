set(SRC
	mainwindow.cpp
	path.cpp
	pathitem.cpp
	task.cpp
	tasklistmodel.cpp
	viewport.cpp

	mainwindow.h
	path.h
	pathitem.h
	task.h
	tasklistmodel.h
	viewport.h
)

add_library(view ${SRC})
