#ifndef AUDIO_PRODUCER_AUDIO_HANDLER_H
#define AUDIO_PRODUCER_AUDIO_HANDLER_H

#include "RtAudio.h"

namespace audio_producer
{

class DeviceHandler;
class StreamBuilder;

/**
 * @brief Wrapper for RtAudio base class, providing multiple helper functions
 */
class AudioHandler
{
public:
    AudioHandler();

    // Rule of 5: we're removing copy operations
    ~AudioHandler() noexcept = default;

    AudioHandler(const AudioHandler&) = delete;
    AudioHandler& operator=(const AudioHandler&) = delete;

    AudioHandler(AudioHandler&&) = default;
    AudioHandler& operator=(AudioHandler&&) = default;

    DeviceHandler getDefaultInputDevice() const;
    DeviceHandler getDefaultOutputDevice() const;
    std::vector<DeviceHandler> getDevices() const;
    StreamBuilder buildStream() const;

private:
    mutable RtAudio audio_{RtAudio::Api::LINUX_ALSA};
};

} // namespace rta_assignment

#endif // AUDIO_PRODUCER_AUDIO_HANDLER_H
