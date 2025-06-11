#include <array>
#include "audio_consumer.h"
#include "fmt/ranges.h"
#include "utils.h"

using namespace ascii_rta;

std::array<float, pipeline::sample_size> generateWhiteNoise()
{
    std::array<float, pipeline::sample_size> result{};

    for (auto i = 0; i < pipeline::sample_size; ++i)
    {
        result[i] = (float(rand()) / RAND_MAX) * 2.0f - 1.0f; // white noise [-1, 1]
    }

    return result;
}

int main()
{
    auto queue = std::make_shared<pipeline::AudioQueue>();
    auto stop_flag = std::make_shared<std::atomic_bool>(false);
    pipeline::AudioConsumer consumer{queue, stop_flag};

    for (auto i = 0; i < 10; ++i)
    {
        const auto samples = generateWhiteNoise();
        queue->try_enqueue(samples);

        std::this_thread::sleep_for(std::chrono::seconds(1));
        fmt::print("Octave Bands: {}\n", consumer.getOctaveBands());
        fflush(stdout);
    }
}
