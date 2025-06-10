#ifndef AUDIO_CONSUMER_H
#define AUDIO_CONSUMER_H

#include <array>
#include <memory>
#include "DspFilters/Filter.h"

namespace audio_consumer
{

static constexpr auto band_count = 10;
static constexpr auto sample_size_ = 8192;

class Analyzer
{
public:
    Analyzer();
    std::array<float, band_count> getOctaveBands();
    void processSamples(std::array<float, sample_size_> samples);

private:
    void _initFilters();

    std::vector<std::unique_ptr<Dsp::Filter>> filters_;

    static constexpr auto sample_rate_ = 44100;
    static constexpr auto filter_order_ = 4;
    static constexpr auto num_channels_ = 1;

    const std::array<float, band_count> octave_frequencies_ = {31.5, 63, 125, 250, 500, 1000, 2000, 4000, 8000, 16000};
    const std::array<float, band_count> band_gains_ = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::array<float, band_count> octave_bands_;
};

} // namespace audio_consumer
#endif // AUDIO_CONSUMER_H
