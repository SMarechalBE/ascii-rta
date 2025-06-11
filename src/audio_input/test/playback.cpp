#include "audio_handler.h"
#include "stream_handler.h"

#include "nonstd/span.hpp"

class SawCallback : public ascii_rta::input::CallbackHandler<float>
{
public:
    explicit SawCallback(const uint8_t channel_count)
        : CallbackHandler{channel_count}, lastValues(channel_count, 0)
    {
    }

    SawCallback(const SawCallback&) = default;

private:
    int process(float* output,
                const float* /*input*/,
                uint32_t frame_count,
                double /*stream_time*/,
                RtAudioStreamStatus status) override
    {
        if (status)
        {
            std::cout << "Stream underflow detected!" << std::endl;
        }

        nonstd::span output_span(output, frame_count * channel_count_);

        // Write interleaved audio data.
        for (uint32_t i = 0; i < frame_count; i++)
        {
            for (uint8_t j = 0; j < channel_count_; j++)
            {
                output_span[i] = lastValues.at(j);

                lastValues.at(j) += .005f * (j + 1 + (j * .1f));
                if (lastValues.at(j) >= 1.0)
                {
                    lastValues.at(j) -= 2.0;
                }
            }
        }

        return 0;
    }

    std::vector<float> lastValues{};
};

int main()
{
    try
    {
        ascii_rta::input::AudioHandler audio{};
        constexpr auto channel_count{1};
        SawCallback saw{channel_count};

        const auto stream = audio.buildStream()
                                    .outputDevice(audio.getDefaultOutputDevice(), channel_count)
                                    .bufferFrames(2048)
                                    .callback(saw)
                                    .autoStart()
                                    .build();

        fmt::print("\nPlaying ... press <enter> to quit.\n\n");
        std::cin.get();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
