#include "audio_pipeline.h"

#include "audio_consumer.h"
#include "audio_producer.h"
#include "utils.h"

#include <memory>

namespace ascii_rta::pipeline
{

AudioPipeline::AudioPipeline()
    : queue_{std::make_shared<AudioQueue>()}
    , stop_flag_{std::make_shared<std::atomic_bool>(false)}
    , producer_{queue_, stop_flag_}
    , consumer_{queue_, stop_flag_}

{
    thread_ = std::thread(&AudioPipeline::_run, this);
}
AudioPipeline::~AudioPipeline()
{
    stop_flag_->store(true);
    if (thread_.joinable())
    {
        thread_.join();
    }
}

std::array<float, 10> AudioPipeline::getOctaveBands() const
{
    return {-90, -80, -70, -60, -50, -40, -30, -20, -10, 0};
    // return consumer_->getOctaveBands();
}

void AudioPipeline::setMic(const bool on) { producer_.setMic(on); }

void AudioPipeline::setSineWave(const bool on) { producer_.setSineWave(on); }

void AudioPipeline::setPinkNoise(const bool on) { producer_.setPinkNoise(on); }

void AudioPipeline::_run() const
{
    input::AudioHandler audio_handler{};
    const auto stream = audio_handler.buildStream()
                                .outputDevice(audio_handler.getDefaultOutputDevice(), audio_channels)
                                .sampleRate(sample_rate)
                                .bufferFrames(sample_size)
                                .callback(producer_)
                                .autoStart()
                                .build();

    while (!stop_flag_->load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

} // namespace ascii_rta::pipeline
