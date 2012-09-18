cmake_minimum_required(VERSION 2.6)
project(GAME_OF_LIFE)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0) # look into FindPkgConfig.cmake,
pkg_check_modules(GLIBMM glibmm-2.4)

#set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x")

set(SFML_LIBS sfml-graphics sfml-window sfml-system)

include_directories(${GTKMM_INCLUDE_DIRS} ${GLIBMM_INCLUDE_DIRS} )

add_executable(life-cmd life.cpp life.hpp main.cpp main.hpp cmd.cpp)
target_link_libraries( life-cmd pthread ${SFML_LIBS})

add_executable(life-gui life.cpp life.hpp main.cpp main.hpp gui.cpp gui.hpp)
target_link_libraries( life-gui pthread ${GLIBMM_LIBRARIES} ${GTKMM_LIBRARIES} ${SFML_LIBS})