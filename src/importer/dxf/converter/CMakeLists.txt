set(SRC
	arc.cpp
	circle.cpp
	line.cpp
	spline.cpp
# 	wpolyline.cpp
# 	polyline.cpp
# 	ellipse.cpp

	arc.h
	circle.h
	line.h
	spline.h
	utils.h
)

add_library(converter ${SRC})

