add_subdirectory(test)

# RtAudio as the Audio backend
FetchContent_Declare(
  dsp-filters
  GIT_REPOSITORY https://github.com/vinniefalco/DSPFilters.git
  GIT_TAG master
  SOURCE_SUBDIR shared/DSPFilters)
FetchContent_MakeAvailable(dsp-filters)

# Audio consumer lib
add_library(audio_consumer audio_consumer.cpp)
target_include_directories(audio_consumer
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(audio_consumer PUBLIC DSPFilters PRIVATE fmt)
