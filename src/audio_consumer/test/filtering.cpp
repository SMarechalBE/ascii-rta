#include "analyzer.h"
#include "fmt/base.h"
#include "fmt/ranges.h"
#include <array>

static constexpr auto sample_size_ = 8192*4;

std::array<float, sample_size_> generateWhiteNoise()
{
    std::array<float, sample_size_> result{};

    for (auto i = 0; i < sample_size_; ++i)
    {
        result[i] = (float(rand()) / RAND_MAX) * 2.0f - 1.0f; // white noise [-1, 1]
    }

    return result;
}

int main()
{
    audio_consumer::Analyzer analyzer{};

    analyzer.processSamples(generateWhiteNoise());

    const auto bands = analyzer.getOctaveBands();
    fmt::print("Bands: {}\n", bands);
}
