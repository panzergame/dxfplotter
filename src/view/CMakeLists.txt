add_subdirectory(settings)
add_subdirectory(task)
add_subdirectory(view2d)

set(SRC
	info.cpp
	mainwindow.cpp

	info.h
	mainwindow.h
)

add_library(view ${SRC})
add_dependencies(view generate_config uic)

