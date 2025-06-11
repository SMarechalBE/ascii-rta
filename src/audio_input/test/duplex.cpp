#include "audio_handler.h"

#include "stream_handler.h"

#include <cstdint>

class EchoCallback : public ascii_rta::input::CallbackHandler<float>
{
    using CallbackHandler::CallbackHandler;

    int process(float* output,
                const float* input,
                uint32_t frame_count,
                double /*stream_time*/,
                RtAudioStreamStatus status) override
    {
        if (status)
        {
            std::cout << "Stream over/underflow detected." << std::endl;
        }

        memcpy(output, input, frame_count * sampleByteSize());

        return 0;
    }
};

int main()
{
    try
    {
        ascii_rta::input::AudioHandler audio{};
        constexpr auto channel_count{1};
        EchoCallback echo{channel_count};

        const auto stream = audio.buildStream()
                                    .outputDevice(audio.getDefaultOutputDevice(), channel_count)
                                    .inputDevice(audio.getDefaultInputDevice(), channel_count)
                                    .bufferFrames(10)
                                    .callback(echo)
                                    .autoStart()
                                    .scheduleRealtime()
                                    .numberOfBuffers(4)
                                    .build();

        fmt::print("\nPlaying ... press <enter> to quit.\n\n");
        std::cin.get();
    }
    catch (const std::exception& e)
    {
        fmt::print("\n{}\n", e.what());
        fflush(stdout);
    }
}
