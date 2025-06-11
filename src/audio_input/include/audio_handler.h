#ifndef AUDIO_INPUT_AUDIO_HANDLER_H
#define AUDIO_INPUT_AUDIO_HANDLER_H

/* RtAudio */
#include "RtAudio.h"

namespace ascii_rta::input
{

class DeviceHandler;
class StreamBuilder;

inline void error_callback(RtAudioErrorType type, const std::string &errorText )
{
    // TODO: Should log in file here
}

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
    mutable RtAudio audio_{RtAudio::Api::LINUX_ALSA, error_callback};
};

} // namespace ascii_rta::input

#endif // AUDIO_INPUT_AUDIO_HANDLER_H
