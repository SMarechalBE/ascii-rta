add_subdirectory(audio_analyzer)
add_subdirectory(audio_input)
add_subdirectory(audio_pipeline)

# sudo apt install libncurses-dev
find_package(Curses REQUIRED)

add_executable(app main.cpp gui.cpp)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app PRIVATE fmt ncurses audio_pipeline)
add_custom_target(
        app_copy_data
        COMMAND
        ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_SOURCE_DIR}/resources/"
        "$<TARGET_FILE_DIR:app>/resources"
        COMMENT "Copying data")
add_dependencies(app app_copy_data)