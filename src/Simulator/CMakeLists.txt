cmake_minimum_required(VERSION 3.5)

file(GLOB_RECURSE SOURCES 
	"*.h"
	"*.hpp"
    "*.cpp"
)

find_package(OpenGL REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PUBLIC ${CMAKE_SOURCE_DIR}/include
)

add_library(glew32s STATIC IMPORTED)
set_target_properties(glew32s PROPERTIES 
	IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/glew/glew32s.lib
)

add_library(glfw3 STATIC IMPORTED)
set_target_properties(glfw3 PROPERTIES 
	IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/glfw3/glfw3.lib
)

target_link_libraries(${CMAKE_PROJECT_NAME}
	PUBLIC ${OPENGL_gl_LIBRARY} 
	PUBLIC glfw3
	PUBLIC glew32s
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/bin)