#include "audio_handler.h"

class EchoCallback : public rta_assignment::CallbackHandler<float>
{
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

        memcpy(output, input, frame_count * sizeof(float) * 2);

        return 0;
    }
};

int main()
{
    try
    {
        rta_assignment::AudioHandler audio{};
        EchoCallback echo{};

        const auto stream = audio.buildStream()
                                    .outputDevice(audio.getDefaultOutputDevice(), 2)
                                    .inputDevice(audio.getDefaultInputDevice(), 2)
                                    .bufferFrames(2048)
                                    .callback(echo)
                                    .autoStart()
                                    .scheduleRealtime()
                                    .minimizeLatency()
                                    .numberOfBuffers(4)
                                    .build();

        fmt::print("\nPlaying ... press <enter> to quit.\n\n");
        std::cin.get();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
