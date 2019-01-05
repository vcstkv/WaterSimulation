cmake_minimum_required(VERSION 3.5)					

file(GLOB SOURCES 
	"include/*.h"
	"include/*.hpp"
    "src/*.cpp"
)

add_library(libgameengine STATIC ${SOURCES})

target_include_directories(libgameengine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(libgameengine PUBLIC libgraphics PUBLIC glfw3)
								