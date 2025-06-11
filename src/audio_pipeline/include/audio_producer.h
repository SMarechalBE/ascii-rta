#ifndef AUDIO_PIPELINE_PRODUCER_H
#define AUDIO_PIPELINE_PRODUCER_H

/* AudioPipeline */
#include "utils.h"

/* AudioInput */
#include "audio_handler.h"

/* STL */
#include <atomic>
#include <memory>
#include <thread>

#include "audio_file.h"
#include "nonstd/span.hpp"
#include "stream_handler.h"

namespace ascii_rta::pipeline
{

class AudioProducer : public input::CallbackHandler<float>
{
public:
    // using CallbackHandler::CallbackHandler;

    explicit AudioProducer(SharedAudioQueue queue, SharedAtomicBool stop_flag)
        : queue_{std::move(queue)}
        , stop_flag_{std::move(stop_flag)}
    {
    }

    void setMic(const bool on) { mic_on_->store(on); }
    void setSineWave(const bool on) { sine_wave_on_->store(on); }
    void setPinkNoise(const bool on) { pink_noise_on_->store(on); }

private:
    int process(float* output,
                const float* input,
                uint32_t frame_count,
                double stream_time,
                RtAudioStreamStatus status) override;

    [[nodiscard]] AudioFrame _computeFrame(nonstd::span<const float> input) const;

    [[nodiscard]] float _getNextPinkNoiseData() const;
    [[nodiscard]] float _getNextSineWaveData() const;

    const std::vector<float> sine_wave_buffer{input::getSineWaveFileData()};
    const std::vector<float> pink_noise_buffer{input::getPinkNoiseFileData()};

    SharedAtomicBool mic_on_{std::make_shared<std::atomic_bool>(false)};
    SharedAtomicBool sine_wave_on_{std::make_shared<std::atomic_bool>(false)};
    SharedAtomicBool pink_noise_on_{std::make_shared<std::atomic_bool>(false)};

    SharedAudioQueue queue_{};
    SharedAtomicBool stop_flag_{};
    std::exception_ptr exception_{};
};

} // namespace ascii_rta::pipeline

#endif // AUDIO_PIPELINE_PRODUCER_H
