#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

#include <string>
#include <vector>

namespace ascii_rta::input
{

std::vector<float> getFileData(const std::string& filepath);


inline std::vector<float> getSineWaveFileData()
{
    return getFileData("./resources/1000.wav");
}

inline std::vector<float> getPinkNoiseFileData()
{
    return getFileData("./resources/pink_mono.wav");
}

} // namespace ascii_rta::input

#endif // AUDIO_FILE_H
