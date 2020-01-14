set(SRC
	mainwindow.cpp
	pathitem.cpp
	task.cpp
	tasklistmodel.cpp
	viewport.cpp

	mainwindow.h
	pathitem.h
	task.h
	tasklistmodel.h
	viewport.h
)

add_library(view ${SRC})
