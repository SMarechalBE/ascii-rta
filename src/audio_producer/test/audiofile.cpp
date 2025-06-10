#define DR_WAV_IMPLEMENTATION
#include <iostream>
#include <vector>
#include "audio_handler.h"
#include "dr_wav.h"
#include "fmt/compile.h"
#include "stream_handler.h"

#include "nonstd/span.hpp"

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
        fmt::print("Failed to load WAV file: {}\n", filepath);

        return {};
    }

    fmt::print("\nLoaded WAV file: {}\n", filepath);
    fmt::print("Channels: {}\n", channels);
    fmt::print("Sample rate: {}\n", sampleRate);
    fmt::print("Total frames: {}\n", totalSampleCount);

    std::vector buffer(samples, samples + totalSampleCount * channels);

    // When done, free the dr_wav buffer
    drwav_free(samples, nullptr);

    return buffer;
}

class SignalCallback : public audio_producer::CallbackHandler<float>
{
public:
    explicit SignalCallback(const uint8_t channel_count, std::vector<float> data)
        : CallbackHandler{channel_count}
        , data_(std::move(data))
    {
    }

private:
    int process(float* output,
                const float* /*input*/,
                uint32_t frame_count,
                double /*stream_time*/,
                RtAudioStreamStatus status) override
    {
        if (status)
        {
            fmt::print("Stream under/overflow detected!\n", status);
        }

        nonstd::span output_span(output, frame_count);
        for (uint32_t i = 0; i < frame_count; i++)
        {
            output_span[i] = data_.at(data_index_);
            data_index_ = ++data_index_ % data_.size(); // loop
        }

        return 0;
    }

    uint32_t data_index_{0};
    std::vector<float> data_{};
};

int main()
{
    // const auto sine_buffer = getFileData("./resources/1000_s16_mono_44100.wav");
    const auto sine_buffer = getFileData("./resources/1000.wav");
    const auto pink_buffer = getFileData("./resources/pink_mono.wav");

    try
    {
        audio_producer::AudioHandler audio{};
        constexpr auto channel_count{1};
        SignalCallback signal{channel_count, pink_buffer};

        const auto stream = audio.buildStream()
                                    .outputDevice(audio.getDefaultOutputDevice(), channel_count)
                                    .bufferFrames(4096)
                                    .callback(signal)
                                    .scheduleRealtime()
                                    .autoStart()
                                    .build();

        fmt::print("\nPlaying ... press <enter> to quit.\n\n");
        std::cin.get();
    }
    catch (const std::exception& e)
    {
        fmt::print("Error: {} \n", e.what());
        fflush(stdout);
    }
}
