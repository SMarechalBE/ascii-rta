#include "audio_handler.h"

/* AudioInput */
#include "device_handler.h"
#include "stream_handler.h"

/* FMT */
#include <fmt/base.h>
#include <fmt/ranges.h>

/* RtAudio */
#include "RtAudio.h"

/* STL */
#include <cstdint>

namespace ascii_rta::input
{

AudioHandler::AudioHandler()
{
    if (audio_.getDeviceIds().empty())
    {
        throw std::runtime_error{"No audio devices found!"};
    }

    audio_.showWarnings(false);
}

DeviceHandler AudioHandler::getDefaultInputDevice() const
{
    return DeviceHandler{audio_.getDeviceInfo(audio_.getDefaultInputDevice())};
}

DeviceHandler AudioHandler::getDefaultOutputDevice() const
{
    return DeviceHandler{audio_.getDeviceInfo(audio_.getDefaultOutputDevice())};
}

std::vector<DeviceHandler> AudioHandler::getDevices() const
{
    const auto device_ids = audio_.getDeviceIds();
    if (device_ids.empty())
    {
        fmt::print("No device found.\n");

        return {};
    }

    std::vector<DeviceHandler> devices{};
    for (const auto& device_id: device_ids)
    {
        devices.emplace_back(audio_.getDeviceInfo(device_id));
    }

    return devices;
}

StreamBuilder AudioHandler::buildStream() const { return StreamBuilder{audio_}; }

} // namespace ascii_rta::input
