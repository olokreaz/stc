cmake_minimum_required(VERSION 3.28)
project(stc)

set(CMAKE_CXX_STANDARD 23)

find_package(glm CONFIG REQUIRED)
find_package(SndFile CONFIG REQUIRED)

find_path(MINIAUDIO_INCLUDE_DIRS "miniaudio.h")


find_package(unofficial-libstk CONFIG REQUIRED)



add_executable(stc_rnd psevdo_random.cpp)
target_link_libraries(stc_rnd PRIVATE glm::glm)

add_executable(stc_gen_sig_itd generate_signals_ITD.cpp)
target_link_libraries(stc_gen_sig_itd PRIVATE glm::glm)
target_link_libraries(stc_gen_sig_itd PRIVATE SndFile::sndfile)
target_include_directories(stc_gen_sig_itd PRIVATE ${MINIAUDIO_INCLUDE_DIRS})


add_executable(stc_gen_sig_k generate_signal_karplus.cpp)
target_link_libraries(stc_gen_sig_k PRIVATE SndFile::sndfile)
target_include_directories(stc_gen_sig_k PRIVATE ${MINIAUDIO_INCLUDE_DIRS})
target_link_libraries(stc_gen_sig_k PRIVATE glm::glm)
