cmake_minimum_required(VERSION 3.28)
project(stc)

set(CMAKE_CXX_STANDARD 23)

find_package(glm CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)

find_package(portaudio REQUIRED)

add_executable(stc_rnd psevdo_random.cpp)

target_link_libraries(stc_rnd PRIVATE glog::glog)
target_link_libraries(stc_rnd PRIVATE glm::glm)


add_executable(stc_generate_signals generate_signals.cpp)
target_link_libraries(stc_generate_signals PRIVATE glog::glog)
target_link_libraries(stc_generate_signals PRIVATE glm::glm)
target_link_libraries(stc_generate_signals PRIVATE portaudio)