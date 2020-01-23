set(SRC
	arc.cpp
	circle.cpp
	line.cpp
# 	wpolyline.cpp
# 	spline.cpp
# 	polyline.cpp
# 	ellipse.cpp

	arc.h
	circle.h
	line.h
	utils.h
)

add_library(converter ${SRC})

