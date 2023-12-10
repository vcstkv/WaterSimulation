cmake_minimum_required(VERSION 3.12)
								
project(WaterSimulation)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 0)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if(MSVC)
	set(CMAKE_CXX_FLAGS "/W3 /WX- /EHsc")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /NDEBUG")
else()
	set(CMAKE_CXX_FLAGS "-W -Wall -Wno-unused -Wno-sign-compare")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

add_subdirectory(lib/GSEngine)

set(SOURCES
    "src/Screens/MainScreen.cpp"
    "src/Screens/SPHScreen.cpp"
    "src/SPH/SPHFluid.cpp"
    "src/SPH/SPHGPUSolver.cpp"
    "src/SPH/SPHSolver.cpp"
    "src/main.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE GSEngine
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_SOURCE_DIR}/bin/data
)
