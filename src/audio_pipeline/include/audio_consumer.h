#ifndef AUDIO_PIPELINE_CONSUMER_H
#define AUDIO_PIPELINE_CONSUMER_H

/* AudioPipeline */
#include "utils.h"

/* STL */
#include <array>
#include <atomic>
#include <memory>
#include <thread>
#include <mutex>

namespace ascii_rta::pipeline
{

class AudioConsumer
{
public:
    explicit AudioConsumer(SharedAudioQueue queue, SharedAtomicBool stop_flag)
        : queue_{std::move(queue)}
        , stop_flag_{std::move(stop_flag)}
        , thread_{&AudioConsumer::run, this}
    {
    }

    ~AudioConsumer();

    [[nodiscard]] std::array<float, 10> getOctaveBands() const;

private:
    void run();

    SharedAudioQueue queue_;
    SharedAtomicBool stop_flag_;
    mutable std::mutex mutex_;
    std::array<float, 10> octave_bands_{};
    std::thread thread_;
};

} // namespace ascii_rta::pipeline

#endif // AUDIO_PIPELINE_CONSUMER_H
