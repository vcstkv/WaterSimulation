cmake_minimum_required(VERSION 3.5)					
								
project(WaterSimulation)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if(MSVC)
	set(CMAKE_CXX_FLAGS "/W3 /WX- /EHsc")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /NDEBUG")
else()
	set(CMAKE_CXX_FLAGS "-W -Wall -Werror -Wno-unused -Wno-sign-compare")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

add_subdirectory(lib/GSEngine)

file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${CMAKE_PROJECT_NAME}
	PRIVATE GSEngine
)
