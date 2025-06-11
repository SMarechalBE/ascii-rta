#include "audio_producer.h"

/* AudioInput */
#include "device_handler.h"
#include "stream_handler.h"

/* AudioPipeline */
#include "utils.h"

/* MoodyCamel */
#include <cstring>

#include "readerwriterqueue.h"

namespace ascii_rta::pipeline
{

float getNextSineWaveData()
{
    static uint32_t current_index = 0;
    static const auto sine_wave_buffer = input::getSineWaveFileData();
    static const auto max_index = sine_wave_buffer.size();

    current_index = (current_index + 1) % max_index;

    return sine_wave_buffer[current_index];
}

float getNextPinkNoiseData()
{
    static uint32_t current_index = 0;
    static const auto pink_noise_buffer = input::getPinkNoiseFileData();
    static const auto max_index = pink_noise_buffer.size();

    current_index = (current_index + 1) % max_index;

    return pink_noise_buffer[current_index];
}

int AudioProducer::process(float* /*output*/,
                           const float* input,
                           uint32_t frame_count,
                           double /*stream_time*/,
                           RtAudioStreamStatus /*status*/)
{
    if (stop_flag_->load(std::memory_order_relaxed))
    {
        return 1;
    }

    if (!input)
    {
        return 0;
    }

    if (frame_count != sample_size)
    {

        return 1;
    }

    queue_->try_enqueue(_computeFrame({input, frame_count}));

    return 0;
}

AudioFrame AudioProducer::_computeFrame(nonstd::span<const float> input) const
{
    const auto mic_on = mic_on_->load(std::memory_order_relaxed);
    const auto sine_wave_on = sine_wave_on_->load(std::memory_order_relaxed);
    const auto pink_noise_on = pink_noise_on_->load(std::memory_order_relaxed);
    const auto source_count = mic_on + sine_wave_on + pink_noise_on;

    if (source_count == 0)
    {
        return {};
    }

    AudioFrame frame{};
    for (auto i = 0; i < sample_size; ++i)
    {
        const auto sample = (mic_on ? input[i] : 0) + (sine_wave_on ? getNextSineWaveData() : 0) +
                       (pink_noise_on ? getNextPinkNoiseData() : 0);
        // Some kind of dynamic gain could be applied here
        const auto normalized_sample = sample / source_count;
        frame[i] = normalized_sample;
    }

    return frame;
}

} // namespace ascii_rta::pipeline
