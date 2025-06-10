# DSPFilters tests

# Filtering
add_executable(filtering filtering.cpp)
target_link_libraries(filtering PRIVATE audio_consumer )
