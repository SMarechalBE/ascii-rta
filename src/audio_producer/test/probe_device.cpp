#include "audio_handler.h"
#include "device_handler.h"
#include "fmt/base.h"

int main()
{
    try
    {
        audio_producer::AudioHandler audio{};

        fmt::print("\n=========== Dumping all devices info ===========\n");
        for (const auto& device: audio.getDevices())
        {
            device.printInfo();
        }

        fmt::print("\n=========== Dumping default INPUT device info ===========\n");
        audio.getDefaultInputDevice().printInfo();

        return 0;
    }
    catch (const std::exception& e)
    {
        fmt::print("{}\n", e.what());
        fflush(stdout);
    }
}
