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

# dr_wav for reading .WAV files
FetchContent_Declare(
  dr_libs
  GIT_REPOSITORY https://github.com/mackron/dr_libs.git
  GIT_TAG master)
FetchContent_MakeAvailable(dr_libs)
add_library(dr_wav INTERFACE)
target_include_directories(dr_wav INTERFACE ${dr_libs_SOURCE_DIR})

# RtAudio Wrapper lib
add_library(audio_input audio_handler.cpp stream_handler.cpp audio_file.cpp)
target_include_directories(audio_input
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(audio_input PUBLIC rtaudio fmt PRIVATE dr_wav)
