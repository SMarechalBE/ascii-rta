#include <iostream>
#include <vector>
#include "audio_handler.h"
#include "audio_file.h"
#include "stream_handler.h"
#include "nonstd/span.hpp"

class SignalCallback : public ascii_rta::input::CallbackHandler<float>
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
    const auto sine_buffer = ascii_rta::input::getSineWaveFileData();
    const auto pink_buffer = ascii_rta::input::getPinkNoiseFileData();

    try
    {
        ascii_rta::input::AudioHandler audio{};
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
