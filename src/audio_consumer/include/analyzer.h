#ifndef AUDIO_CONSUMER_ANALYZER_H
#define AUDIO_CONSUMER_ANALYZER_H

/* DSPFilters */
#include "DspFilters/Filter.h"

/* Span-lite */
#include "nonstd/span.hpp"

/* STL */
#include <array>
#include <memory>
#include <vector>

namespace audio_consumer
{

static constexpr auto band_count = 10;

class Analyzer
{
public:
    Analyzer();
    explicit Analyzer(int sample_rate);

    [[nodiscard]] std::array<float, band_count> getOctaveBands() const { return octave_bands_; }
    void processSamples(nonstd::span<const float> samples);

private:
    void _initFilters();

    std::vector<std::unique_ptr<Dsp::Filter>> filters_;

    const int sample_rate_{44100};
    static constexpr auto filter_order_ = 4;
    static constexpr auto num_channels_ = 1;

    const std::array<float, band_count> octave_frequencies_ = {31.5, 63, 125, 250, 500, 1000, 2000, 4000, 8000, 16000};
    const std::array<float, band_count> band_gains_ = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::array<float, band_count> octave_bands_{};
};

} // namespace audio_consumer

#endif // AUDIO_CONSUMER_ANALYZER_H
