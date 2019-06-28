cmake_minimum_required(VERSION 3.5)

file(GLOB_RECURSE SOURCES 
	"*.h"
	"*.hpp"
    "*.cpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

#find_package(OpenGL REQUIRED)
#add_library(glew32s STATIC IMPORTED)
#set_target_properties(glew32s PROPERTIES 
#	IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/glew/glew32s.lib
#)

target_include_directories(${CMAKE_PROJECT_NAME} 
#	PUBLIC ${OPENGL_INCLUDE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${CMAKE_PROJECT_NAME} 
	PUBLIC libfilesio
	PUBLIC libgraphics
	PUBLIC libgui
#	PUBLIC ${OPENGL_gl_LIBRARY}
#	PUBLIC glew32s
)

add_custom_command(
	TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} 
	-E copy $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/bin
)