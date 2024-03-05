cmake_minimum_required(VERSION 3.28)
project(stc)

set(CMAKE_CXX_STANDARD 23)

find_package(glm CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)

find_package(portaudio REQUIRED)
find_package(SndFile REQUIRED)


add_executable(stc_rnd psevdo_random.cpp)

target_link_libraries(stc_rnd PRIVATE glog::glog)
target_link_libraries(stc_rnd PRIVATE glm::glm)

add_executable(stc_gen_sig_itd generate_signals_ITD.cpp)
target_link_libraries(stc_gen_sig_itd PRIVATE glog::glog)
target_link_libraries(stc_gen_sig_itd PRIVATE glm::glm)
target_link_libraries(stc_gen_sig_itd PRIVATE portaudio)

add_executable(stc_gen_sig_k generate_signal_karplus.cpp)
target_link_libraries(stc_gen_sig_k PRIVATE SndFile::sndfile)
target_link_libraries(stc_gen_sig_k PRIVATE portaudio)
target_link_libraries(stc_gen_sig_k PRIVATE glog::glog)
