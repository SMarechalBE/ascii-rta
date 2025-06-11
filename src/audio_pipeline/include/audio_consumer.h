#ifndef AUDIO_PIPELINE_CONSUMER_H
#define AUDIO_PIPELINE_CONSUMER_H

/* AudioPipeline */
#include "utils.h"

/* STL */
#include <atomic>
#include <memory>
#include <thread>

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

private:
    void run();

    SharedAudioQueue queue_;
    SharedAtomicBool stop_flag_;
    std::thread thread_;
};

} // namespace ascii_rta::pipeline

#endif // AUDIO_PIPELINE_CONSUMER_H
