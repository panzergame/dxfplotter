set(SRC
	arc.cpp
	circle.cpp
	line.cpp
 	lwpolyline.cpp
	spline.cpp
# 	polyline.cpp
# 	ellipse.cpp

	arc.h
	circle.h
	line.h
 	lwpolyline.h
	spline.h
	utils.h
)

add_library(converter ${SRC})

