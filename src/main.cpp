#include <cstdio>
#include "audio_consumer.h"
#include "gui.h"
#include "ncurses.h"

int main()
{
    Gui gui{};
    audio_consumer::Analyzer analyzer{};

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
                // audio.setSine(gui.sine_on_)
                break;
            case '2':
                gui.pink_on_ ^= true;
                // audio.setPink(gui.pink_on_)
                break;
            case '3':
                gui.mic_on_ ^= true;
                // audio.setMic(gui.mic_on_)
                break;
            default:;
        }
        fflush(stdout);

        const auto data = analyzer.getOctaveBands();
        gui.draw(data);
    }

    return 0;
}
