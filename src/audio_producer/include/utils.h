#ifndef AUDIO_PRODUCER_UTILS_H
#define AUDIO_PRODUCER_UTILS_H

/* RtAudio */
#include "RtAudio.h"

namespace audio_producer
{

using DeviceId = unsigned int;
using SampleRate = unsigned int;

constexpr std::string_view toString(const RtAudioErrorType type)
{
    switch (type)
    {
        case RTAUDIO_NO_ERROR:
            return "No error";
        case RTAUDIO_WARNING:
            return "Non-critical error.";
        case RTAUDIO_UNKNOWN_ERROR:
            return "Unspecified error type.";
        case RTAUDIO_NO_DEVICES_FOUND:
            return "No devices found on system";
        case RTAUDIO_INVALID_DEVICE:
            return "An invalid device ID was specified.";
        case RTAUDIO_DEVICE_DISCONNECT:
            return "A device in use was disconnected.";
        case RTAUDIO_MEMORY_ERROR:
            return "An error occurred during memory allocation.";
        case RTAUDIO_INVALID_PARAMETER:
            return "An invalid parameter was specified to a function.";
        case RTAUDIO_INVALID_USE:
            return "The function was called incorrectly.";
        case RTAUDIO_DRIVER_ERROR:
            return "A system driver error occurred.";
        case RTAUDIO_SYSTEM_ERROR:
            return "A system error occurred.";
        case RTAUDIO_THREAD_ERROR:
            return "A thread error occurred.";
        default:
            return "Unknown error type.";
    }
}

}

#endif // AUDIO_PRODUCER_UTILS_H
