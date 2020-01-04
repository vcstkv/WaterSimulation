cmake_minimum_required(VERSION 3.5)					
								
project(WaterSimulation LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if(MSVC)
	add_compile_options(/W3 /WX-)
else()
	add_compile_options(-W -Wall -Werror)
endif()

add_subdirectory(src/FilesIOLib)
add_subdirectory(src/GraphicsLib)
add_subdirectory(src/GUILib)
add_subdirectory(src/Simulator)