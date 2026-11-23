set(SRC
	tooloperation.cpp
	offset.cpp
	pocket.cpp
	dragknifeentry.cpp
	dragknife.cpp
)

add_module_library(geometry-tooloperation ${SRC})
target_link_libraries(geometry-tooloperation PUBLIC geometry PRIVATE geometry-filter)
