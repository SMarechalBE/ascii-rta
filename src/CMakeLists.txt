add_subdirectory(audio_analyzer)
add_subdirectory(audio_producer)

# sudo apt install libncurses-dev
find_package(Curses REQUIRED)

add_executable(app main.cpp gui.cpp)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app PRIVATE fmt ncurses audio_consumer)
