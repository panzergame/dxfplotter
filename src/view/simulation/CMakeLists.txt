add_subdirectory(internal)

set(SRC
	simulation.cpp
)

add_module_library(view-simulation ${SRC})
target_link_libraries(view-simulation PUBLIC model model-application PRIVATE config geometry view-simulation-internal)
add_dependencies(view-simulation uic)
