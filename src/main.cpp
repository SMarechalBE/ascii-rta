#include <cstdio>
#include "gui.h"
#include "ncurses.h"
#include "audio_pipeline.h"

int main()
{
    Gui gui{};
    ascii_rta::pipeline::AudioPipeline audio_pipeline{};

    auto running{true};
    while (running)
    {
        switch (getch())
        {
            case 'q':
                running = false;
                break;
            case '1':
                gui.sine_on_ ^= true;
                audio_pipeline.setSineWave(gui.sine_on_);
                break;
            case '2':
                gui.pink_on_ ^= true;
                audio_pipeline.setPinkNoise(gui.pink_on_);
                break;
            case '3':
                gui.mic_on_ ^= true;
                audio_pipeline.setMic(gui.mic_on_);
                break;
            default:;
        }
        fflush(stdout);

        gui.draw(audio_pipeline.getOctaveBands());
    }

    return 0;
}
