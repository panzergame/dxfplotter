qt5_add_resources(RESOURCES
	font.qrc
)

message(${RESOURCES})

add_library(font-resource ${RESOURCES})
