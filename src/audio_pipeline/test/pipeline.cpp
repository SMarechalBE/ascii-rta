#include <memory>
#include "audio_pipeline.h"
#include "fmt/os.h"
#include "fmt/ranges.h"
#include "utils.h"

using namespace ascii_rta;

int main()
{
    pipeline::AudioPipeline pipeline{};
    pipeline.setPinkNoise(true);
    for (auto i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        fmt::print("Octave Bands: {}\n", pipeline.getOctaveBands());
        fflush(stdout);
    }
}
