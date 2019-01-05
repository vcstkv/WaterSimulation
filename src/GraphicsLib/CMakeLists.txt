cmake_minimum_required(VERSION 3.5)					

file(GLOB SOURCES 
	"include/*.h"
	"include/*.hpp"
    "src/*.cpp"
)

find_package(OpenGL REQUIRED)

add_library(libgraphics STATIC ${SOURCES})

target_include_directories(libgraphics PUBLIC ${OPENGL_INCLUDE_DIR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(libgraphics PUBLIC ${OPENGL_gl_LIBRARY} PUBLIC glm_static PUBLIC glew)					