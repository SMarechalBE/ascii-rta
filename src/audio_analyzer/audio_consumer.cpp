#include "audio_consumer.h"

#include "DspFilters/Butterworth.h"
#include "fmt/base.h"

namespace audio_consumer
{

Analyzer::Analyzer() { _initFilters(); }

std::array<float, band_count> Analyzer::getOctaveBands()
{
    // return std::array<float, band_count>{0, -10, -20, -30, -40, -50, -60, -70, -80, -90};
    return octave_bands_;
}

void Analyzer::processSamples(std::array<float, sample_size_> samples)
{
    for (auto i = 0; i < band_count; ++i) // Note this could be processed in parallel
    {
        float* audioData[1];
        audioData[0] = new float[sample_size_];
        memcpy(audioData[0], samples.data(), sizeof(samples));

        filters_[i]->process(sample_size_, audioData);

        // RMS power
        double sumSq = 0;
        for (int i = 0; i < sample_size_; ++i)
        {
            sumSq += audioData[0][i] * audioData[0][i];
        }
        delete[] audioData[0];
        double rms = std::sqrt(sumSq / sample_size_);
        fmt::print("Octave[{}] RMS: {}", octave_frequencies_[i], rms);
        double db = 20 * std::log10(rms + 1e-10); // avoid log(0)

        octave_bands_[i] = static_cast<float>(db);
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
