#include "audio_file.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

namespace ascii_rta::input
{

std::vector<float> getFileData(const std::string& filepath)
{
    unsigned int channels;
    unsigned int sampleRate;
    drwav_uint64 totalSampleCount;
    const auto samples = drwav_open_file_and_read_pcm_frames_f32(filepath.c_str(),
                                                                 &channels,
                                                                 &sampleRate,
                                                                 &totalSampleCount,
                                                                 nullptr);
    if (samples == nullptr)
    {
        return {};
    }

    std::vector buffer(samples, samples + totalSampleCount * channels);

    drwav_free(samples, nullptr);

    return buffer;
}

} // namespace ascii_rta::input
