add_subdirectory(audio_analyzer)
add_subdirectory(audio_producer)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE fmt)
