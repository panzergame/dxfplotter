add_subdirectory(dialogs)
add_subdirectory(task)
add_subdirectory(view2d)

if (WITH_3D)
	add_subdirectory(simulation)
endif()

set(SRC
	info.cpp
	mainwindow.cpp
	profile.cpp

	info.h
	mainwindow.h
	profile.h
)

add_library(view ${SRC})
add_dependencies(view generate-config uic)

