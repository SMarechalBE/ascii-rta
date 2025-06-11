#include "audio_consumer.h"

#include "analyzer.h"

namespace ascii_rta::pipeline
{

AudioConsumer::~AudioConsumer()
{
    stop_flag_->store(true);
    if (thread_.joinable())
    {
        thread_.join();
    }
}

std::array<float, 10> AudioConsumer::getOctaveBands() const
{
    std::lock_guard lock(mutex_);

    return octave_bands_;
}

void AudioConsumer::run()
{
    analyzer::Analyzer analyzer{sample_rate};

    while (!(*stop_flag_))
    {
        AudioFrame samples;

        while (queue_->try_dequeue(samples))
        {
            analyzer.processSamples(samples);
            const auto bands = analyzer.getOctaveBands();

            {
                std::lock_guard lock(mutex_);
                octave_bands_ = bands;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

} // namespace ascii_rta::pipeline
