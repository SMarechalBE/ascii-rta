#include "analyzer.h"

#include "DspFilters/Butterworth.h"

namespace audio_consumer
{

Analyzer::Analyzer() { _initFilters(); }

Analyzer::Analyzer(int sample_rate)
    : sample_rate_{sample_rate}
{
    _initFilters();
}

void Analyzer::processSamples(nonstd::span<const float> samples)
{
    // Note: this could be processed in parallel but may be overkill for this task
    for (auto band_idx = 0; band_idx < band_count; ++band_idx)
    {
        // We have to create a local copy of the samples since filter::process() overwrites in place
        std::vector<float> samples_copy{samples.cbegin(), samples.cend()};

        float* audioData[num_channels_] = {samples_copy.data()};
        const auto samples_size = static_cast<int>(samples.size());
        filters_[band_idx]->process(samples_size, audioData);

        // RMS power
        double sumSq = 0;
        for (auto j = 0; j < samples_size; ++j)
        {
            sumSq += samples_copy[j] * samples_copy[j];
        }
        double rms = std::sqrt(sumSq / samples_size);
        double db = 20 * std::log10(rms + 1e-10); // avoid log(0)

        octave_bands_[band_idx] = static_cast<float>(db);
    }
}

void Analyzer::_initFilters()
{
    for (const auto center_frequency: octave_frequencies_)
    {
        auto filter =
                std::make_unique<Dsp::FilterDesign<Dsp::Butterworth::Design::BandPass<filter_order_>, num_channels_>>();

        const auto bandwidth = center_frequency / std::sqrt(2.0);

        Dsp::Params params{};
        params[0] = sample_rate_;
        params[1] = filter_order_;
        params[2] = center_frequency;
        params[3] = bandwidth;
        filter->setParams(params);

        filters_.emplace_back(std::move(filter));
    }
}

} // namespace audio_consumer
