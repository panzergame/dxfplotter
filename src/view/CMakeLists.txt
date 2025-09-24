add_subdirectory(dialogs)
add_subdirectory(task)
add_subdirectory(view2d)
add_subdirectory(simulation)

set(SRC
	info.cpp
	mainwindow.cpp
	profile.cpp

	info.h
	mainwindow.h
	profile.h
)

add_library(view ${SRC})
target_link_libraries(view PRIVATE config)
add_dependencies(view uic)

