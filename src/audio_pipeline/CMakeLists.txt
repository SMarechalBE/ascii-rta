add_subdirectory(test)

FetchContent_Declare(
  readerwriterqueue
  GIT_REPOSITORY https://github.com/cameron314/readerwriterqueue
  GIT_TAG master)
FetchContent_MakeAvailable(readerwriterqueue)

add_library(audio_pipeline audio_consumer.cpp audio_producer.cpp audio_pipeline.cpp)
target_include_directories(audio_pipeline PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(audio_pipeline PUBLIC readerwriterqueue audio_input audio_analyzer)
add_custom_target(
        audio_pipeline_copy_data
        COMMAND
        ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_SOURCE_DIR}/resources/"
        "$<TARGET_FILE_DIR:audio_pipeline>/resources"
        COMMENT "Copying data")
add_dependencies(audio_pipeline audio_pipeline_copy_data)
