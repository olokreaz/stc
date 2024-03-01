cmake_minimum_required(VERSION 3.28)
project(stc)

set(CMAKE_CXX_STANDARD 23)

add_executable(stc_rnd psevdo_random.cpp)

find_package(glm CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)

target_link_libraries(stc_rnd PRIVATE glog::glog)
target_link_libraries(stc_rnd PRIVATE glm::glm)
