add_subdirectory(settings)
add_subdirectory(task)
add_subdirectory(view2d)

set(SRC
	info.cpp
	mainwindow.cpp
	profile.cpp

	info.h
	mainwindow.h
	profile.h
)

add_library(view ${SRC})
add_dependencies(view generate_config uic uic-settings)

