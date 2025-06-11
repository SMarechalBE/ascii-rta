#ifndef AUDIO_PIPELINE_QUEUE_H
#define AUDIO_PIPELINE_QUEUE_H
#include <array>
#include "readerwriterqueue.h"

namespace ascii_rta::pipeline
{

using SharedAtomicBool = std::shared_ptr<std::atomic_bool>;

// Audio config
static constexpr auto sample_size{1024};
static constexpr auto sample_rate{44100};
static constexpr auto audio_channels{1};

// Queue config
static constexpr auto queue_size{256};
using AudioFrame = std::array<float, sample_size>;
using AudioQueue = moodycamel::ReaderWriterQueue<AudioFrame>;
using SharedAudioQueue = std::shared_ptr<AudioQueue>;

} // namespace ascii_rta::pipeline

#endif // AUDIO_PIPELINE_QUEUE_H
