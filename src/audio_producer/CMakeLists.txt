add_subdirectory(test)

# RtAudio as the Audio backend
FetchContent_Declare(
  rtaudio
  GIT_REPOSITORY https://github.com/thestk/rtaudio.git
  GIT_TAG master)
FetchContent_MakeAvailable(rtaudio)

# I only tested with ALSA, so I'm forcing its use here
find_package(ALSA)
if(NOT ALSA_FOUND)
  message(
    FATAL_ERROR
      "Missing ALSA dev files: install with: sudo apt install libasound2-dev")
endif()

# Span library for C++11 and more, helps with navigation for audio buffers
FetchContent_Declare(
  span-lite
  GIT_REPOSITORY https://github.com/martinmoene/span-lite.git
  GIT_TAG master)
FetchContent_MakeAvailable(span-lite)

# RtAudio Wrapper lib
add_library(audio_producer audio_handler.cpp stream_handler.cpp)
target_include_directories(audio_producer
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(
  audio_producer
  PUBLIC rtaudio
  PRIVATE fmt)
