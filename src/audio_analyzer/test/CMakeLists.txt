# DSPFilters tests

# Filtering
add_executable(tst_filtering filtering.cpp)
target_link_libraries(tst_filtering PRIVATE audio_analyzer fmt)
