#include "audio_handler.h"
#include "stream_handler.h"

#include "nonstd/span.hpp"

class SawCallback : public audio_producer::CallbackHandler<float>
{
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

        nonstd::span output_span(output, frame_count*2);

        // Write interleaved audio data.
        for (uint32_t i = 0; i < frame_count; i++)
        {
            for (uint8_t j = 0; j < 2; j++)
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

    std::array<float, 2> lastValues{0, 0};
};

int main()
{
    try
    {
        audio_producer::AudioHandler audio{};
        SawCallback sawcb{};

        const auto stream = audio.buildStream()
                                    .outputDevice(audio.getDefaultOutputDevice(), 2)
                                    .bufferFrames(512)
                                    .callback(sawcb)
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
