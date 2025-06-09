# Probe devices
add_executable(probe_device probe_device.cpp)
target_link_libraries(probe_device PRIVATE audio_handler fmt)

# Playback
add_executable(playback playback.cpp)
target_link_libraries(playback PRIVATE rtaudio audio_handler fmt span-lite)

# Duplex
add_executable(duplex duplex.cpp)
target_link_libraries(duplex PRIVATE rtaudio audio_handler fmt)
