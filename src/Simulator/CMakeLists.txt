cmake_minimum_required(VERSION 3.5)

file(GLOB_RECURSE SOURCES 
	"*.h"
	"*.hpp"
    "*.cpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${CMAKE_PROJECT_NAME} 
	PUBLIC libfilesio
	PUBLIC libgraphics
	PUBLIC libgui
)

add_custom_command(
	TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} 
	-E copy $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/bin
)