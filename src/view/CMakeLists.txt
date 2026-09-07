add_subdirectory(dialogs)
add_subdirectory(task)
add_subdirectory(view2d)
add_subdirectory(simulation)

set(SRC
	info.cpp
	mainwindow.cpp
	profile.cpp
	transform.cpp
)

add_module_library(view ${SRC})
target_link_libraries(view
	PUBLIC model model-application
	PRIVATE config geometry
		view-dialogs view-dialogs-settings view-task view-view2d view-simulation)
add_dependencies(view uic)

