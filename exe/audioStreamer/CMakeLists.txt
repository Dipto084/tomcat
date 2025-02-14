project(audioStreamer)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_definitions(BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(portaudio REQUIRED)

add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(
    Boost 
    COMPONENTS 
    program_options
    log
    REQUIRED
)

add_executable(audioStreamer 
	audioStreamer.cpp	
)

target_include_directories(audioStreamer PRIVATE 
    ${portaudio_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
)

target_link_libraries(
    audioStreamer
    ${Boost_LIBRARIES}
    ${portaudio_LIBRARY}
    -pthread
)
