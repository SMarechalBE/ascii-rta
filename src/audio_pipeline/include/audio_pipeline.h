#ifndef AUDIO_PIPELINE_H
#define AUDIO_PIPELINE_H

/* AudioPipeline */
#include "audio_consumer.h"
#include "audio_producer.h"

/* STL */
#include <array>
#include <atomic>
#include <memory>
#include <thread>

#include "utils.h"

namespace ascii_rta::pipeline
{

/**
 * @brief Acts as a facade for the audio threads so that any UI can interface on them, it is also responsible for their
 * creation and overall lifetime
 */
class AudioPipeline
{
public:
    AudioPipeline();

    ~AudioPipeline();

    [[nodiscard]]std::array<float, 10> getOctaveBands() const;

    void setMic(bool on);
    void setSineWave(bool on);
    void setPinkNoise(bool on);

private:
    void _run() const;

    std::thread thread_{};
    SharedAudioQueue queue_{};
    SharedAtomicBool stop_flag_{};
    AudioProducer producer_;
    AudioConsumer consumer_;
};

} // namespace ascii_rta::pipeline

#endif // AUDIO_PIPELINE_H
