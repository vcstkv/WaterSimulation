cmake_minimum_required(VERSION 3.5)					

file(GLOB_RECURSE SOURCES 
	"*.h"
	"*.hpp"
    "*.cpp"
)

find_package(OpenGL REQUIRED)

add_library(glew32s STATIC IMPORTED)
set_target_properties(glew32s PROPERTIES 
	IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/glew/glew32s.lib
)

add_library(libgraphics SHARED ${SOURCES})
target_include_directories(libgraphics 
	PUBLIC ${OPENGL_INCLUDE_DIR} 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(libgraphics 
	PUBLIC libfilesio
	PUBLIC ${OPENGL_gl_LIBRARY}
	PUBLIC glew32s
)

add_custom_command(
	TARGET libgraphics POST_BUILD 
    COMMAND ${CMAKE_COMMAND} 
	-E copy $<TARGET_FILE:libgraphics> ${CMAKE_SOURCE_DIR}/bin 
)				