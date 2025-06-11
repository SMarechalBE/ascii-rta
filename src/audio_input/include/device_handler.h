#ifndef AUDIO_INPUT_DEVICE_HANDLER_H
#define AUDIO_INPUT_DEVICE_HANDLER_H

/* FMT */
#include "fmt/base.h"
#include "fmt/ranges.h"

/* RtAudio */
#include "RtAudio.h"

namespace ascii_rta::input
{

using DeviceId = unsigned int;
using SampleRate = unsigned int;

class DeviceHandler
{
public:
    explicit DeviceHandler(RtAudio::DeviceInfo device_info)
        : device_{std::move(device_info)}
    {
    }

    [[nodiscard]] DeviceId id() const { return device_.ID; }

    [[nodiscard]] bool supports(const SampleRate rate) const
    {
        return std::any_of(device_.sampleRates.cbegin(),
                           device_.sampleRates.cend(),
                           [=](const SampleRate sample_rate) { return rate == sample_rate; });
    }

    void printInfo() const
    {
        fmt::print("Device: {} [{}] (Default Output:{} Input:{})\n",
                   device_.name,
                   device_.ID,
                   (device_.isDefaultOutput ? "YES" : "NO"),
                   (device_.isDefaultInput ? "YES" : "NO"));
        fmt::print(" Max channels: [output:{}, input:{}, duplex:{}]\n",
                   device_.outputChannels,
                   device_.inputChannels,
                   device_.duplexChannels);
        fmt::print(" Supported sample rates: {}\n", device_.sampleRates);
        fmt::print(" Current sample rate: {}\n", device_.currentSampleRate);
        fmt::print("Supported formats: {}\n", device_.nativeFormats);
        fflush(stdout);
    }

private:
    const RtAudio::DeviceInfo device_;
};

} // namespace ascii_rta::input

#endif // AUDIO_INPUT_DEVICE_HANDLER_H
