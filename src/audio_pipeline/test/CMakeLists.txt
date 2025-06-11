# Audio pipeline tests

# Consumer
add_executable(tst_consumer consumer.cpp)
target_link_libraries(tst_consumer PRIVATE audio_pipeline fmt)

# Pipeline
add_executable(tst_pipeline pipeline.cpp)
target_link_libraries(tst_pipeline PRIVATE audio_pipeline fmt)
add_custom_target(
        tst_pipeline_copy_data
        COMMAND
        ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_SOURCE_DIR}/resources/"
        "$<TARGET_FILE_DIR:tst_pipeline>/resources"
        COMMENT "Copying data")
add_dependencies(tst_pipeline tst_pipeline_copy_data)