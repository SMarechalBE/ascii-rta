# RtAudio examples applied to audio_handler module

# Probe devices
add_executable(tst_probe_device probe_device.cpp)
target_link_libraries(tst_probe_device PRIVATE audio_producer fmt)

# Playback
add_executable(tst_playback playback.cpp)
target_link_libraries(tst_playback PRIVATE rtaudio audio_producer fmt span-lite)

# Duplex
add_executable(tst_duplex duplex.cpp)
target_link_libraries(tst_duplex PRIVATE rtaudio audio_producer fmt)

# dr_wav for reading .WAV files
FetchContent_Declare(
        dr_libs
        GIT_REPOSITORY https://github.com/mackron/dr_libs.git
        GIT_TAG master)
FetchContent_MakeAvailable(dr_libs)
add_library(dr_wav INTERFACE)
target_include_directories(dr_wav INTERFACE ${dr_libs_SOURCE_DIR})

# Audio file
add_executable(tst_audio_file audiofile.cpp)
target_link_libraries(tst_audio_file PRIVATE rtaudio audio_producer fmt dr_wav nonstd::span-lite)
add_custom_target(copy_data
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_SOURCE_DIR}/resources/"
        "$<TARGET_FILE_DIR:tst_audio_file>/resources"
        COMMENT "Copying data"
)
add_dependencies(tst_audio_file copy_data)
