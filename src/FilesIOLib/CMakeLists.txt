cmake_minimum_required(VERSION 3.5)					

file(GLOB_RECURSE SOURCES 
	"*.h"
	"*.hpp"
    "*.cpp"
)

add_library(libfilesio SHARED ${SOURCES})

target_include_directories(libfilesio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_custom_command(
	TARGET libfilesio POST_BUILD
    COMMAND ${CMAKE_COMMAND} 
	-E copy $<TARGET_FILE:libfilesio> ${CMAKE_SOURCE_DIR}/bin
)