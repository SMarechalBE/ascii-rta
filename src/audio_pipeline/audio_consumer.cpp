#include "audio_consumer.h"

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

void AudioConsumer::run()
{
    while (!stop_flag_->load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

} // namespace ascii_rta::pipeline
