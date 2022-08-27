add_subdirectory(settings)

set(SRC
	mirror.cpp
	transform.cpp
	setorigin.cpp

	mirror.h
	transform.h
	setorigin.h
)

add_library(view-dialogs ${SRC})
add_dependencies(view-dialogs uic-dialogs)

