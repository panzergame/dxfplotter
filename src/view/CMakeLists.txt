add_subdirectory(dialogs)
add_subdirectory(task)
add_subdirectory(view2d)
if (WITH_SIMULATION)
	add_subdirectory(simulation)
endif()

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
		view-dialogs view-dialogs-settings view-task view-view2d)
if (WITH_SIMULATION)
	target_link_libraries(view PRIVATE view-simulation)
	target_compile_definitions(view PRIVATE WITH_SIMULATION)
endif()
add_dependencies(view uic)
