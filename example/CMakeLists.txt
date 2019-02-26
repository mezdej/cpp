# CMakeList.txt : CMake project for example, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME ON) 
find_package(Boost 1.66) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
	link_directories(${Boost_LIBRARY_DIRS})
    add_definitions(${Boost_DEFINITIONS})
    add_executable(example "main.cpp"
	"home.h" "home.cpp"
	"device.h" "device.cpp"
	"engines/interface.h" "engines/interface.cpp"
	"engines/rest.h" "engines/rest.cpp"
	"engines/mock.h" "engines/mock.cpp"
	"network/http.h" "network/http.cpp"
	"network/url.h" "network/url.cpp"
	) 
    target_link_libraries(example ${Boost_LIBRARIES})
endif()