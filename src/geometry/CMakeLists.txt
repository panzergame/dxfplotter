add_subdirectory(filter)
add_subdirectory(tooloperation)

set(SRC
	utils.cpp
	circle.cpp
	arc.cpp
	line.cpp
	rect.cpp
	bulge.cpp
	polyline.cpp
	biarc.cpp
	cavc.cpp
	bezier.cpp
	spline.cpp
	cubicspline.cpp
	quadraticspline.cpp
	pocketer.cpp
)

set(NON_MODULE_SRC)

if (WITH_ORTOOLS)
	list(APPEND SRC
		orderoptimizer.ixx
	)
	list(APPEND NON_MODULE_SRC
		orderoptimizer.cpp
	)
endif()

add_module_library(geometry ${SRC} NON_MODULE_SOURCES ${NON_MODULE_SRC})
target_link_libraries(geometry PRIVATE common serializer_access)
if (WITH_ORTOOLS)
	target_link_libraries(geometry PUBLIC ortools::ortools)
	target_compile_definitions(geometry PUBLIC WITH_ORTOOLS)
endif()
