cmake_minimum_required(VERSION 3.5)					

set (CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES 
	"*.h"
	"*.hpp"
    "*.cpp"
)

add_library(glfw3 STATIC IMPORTED)
set_target_properties(glfw3 PROPERTIES 
	IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/glfw3/glfw3.lib
)

add_library(libgui SHARED ${SOURCES})

target_include_directories(libgui 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries( libgui 
	PUBLIC libfilesio
	PUBLIC libgraphics 
	PUBLIC glfw3
)

#add_custom_command(
#	TARGET libgui POST_BUILD 
#    COMMAND ${CMAKE_COMMAND} 
#	-E copy $<TARGET_FILE:libgui> ${CMAKE_SOURCE_DIR}/bin 
#)					