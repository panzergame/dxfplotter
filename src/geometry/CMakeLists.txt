add_subdirectory(filter)

set(SRC
	arc.cpp
	bezier.cpp
	biarc.cpp
	bulge.cpp
	circle.cpp
	cubicspline.cpp
	line.cpp
	pocketer.cpp
	polyline.cpp
	quadraticspline.cpp
	rect.cpp
	spline.cpp

	arc.h
	bezier.h
	biarc.h
	bulge.h
	circle.h
	cubicspline.h
	line.h
	polyline.h
	quadraticspline.h
	spline.h
	rect.h
	utils.h
)

if (WITH_ORTOOLS)
	list(APPEND SRC
		orderoptimizer.cpp

		orderoptimizer.h
	)
endif()

add_library(geometry ${SRC})
if (WITH_ORTOOLS)
	target_link_libraries(geometry PUBLIC ortools::ortools)
	target_compile_definitions(geometry PUBLIC WITH_ORTOOLS)
endif()


add_coverage(geometry)
