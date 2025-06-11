#include "stream_handler.h"

/* FMT */
#include <fmt/format.h>

/* RtAudio */
#include "RtAudio.h"

/* STL */
#include <cstdint>

namespace
{
template<typename T>
T* as_ptr(std::optional<T>& opt) noexcept
{
    return opt.has_value() ? std::addressof(opt.value()) : nullptr;
}

template<typename T>
const T* as_ptr(const std::optional<T>& opt) noexcept
{
    return opt.has_value() ? std::addressof(opt.value()) : nullptr;
}

} // namespace

namespace ascii_rta::input
{

StreamHandler::StreamHandler(const StreamBuilder& builder)
    : audio_{builder.audio_}
    , sample_rate_{builder.sample_rate_}
    , output_parameters_{_initParameter(builder.output_device_, builder.output_channel_count_, builder.sample_rate_)}
    , input_parameters_{_initParameter(builder.input_device_, builder.input_channel_count_, builder.sample_rate_)}
    , buffer_frames_{builder.buffer_frames_}
    , callback_{builder.callback_}
{
    _doOpen();

    if (builder.auto_start_)
    {
        _doStart();
    }
}

StreamHandler::~StreamHandler() noexcept
{
    stop();

    if (audio_.isStreamOpen())
    {
        audio_.closeStream();
    }
}

void StreamHandler::start()
{
    if (audio_.isStreamRunning())
    {
        return;
    }

    _doStart();
}

void StreamHandler::stop() noexcept
{
    if (!audio_.isStreamRunning())
    {
        return;
    }

    audio_.stopStream();
}

std::optional<RtAudio::StreamParameters> StreamHandler::_initParameter(const std::optional<DeviceHandler>& device,
                                                                       const uint32_t channel,
                                                                       const SampleRate sample_rate)
{
    if (!device)
    {
        return std::nullopt;
    }

    if (!device->supports(sample_rate))
    {
        throw StreamException{fmt::format("Device[{}] does not support sample rate {}", device->id(), sample_rate)};
    }

    return std::make_optional<RtAudio::StreamParameters>({device->id(), channel});
}

void StreamHandler::_doStart()
{
    if (const auto err = audio_.startStream())
    {
        throw StreamException{audio_.getErrorText()};
    }
}

void StreamHandler::_doOpen()
{

    fmt::print("Opening stream, sample rate:{}, desired buffer frames:{}", sample_rate_, buffer_frames_);
    const auto desired_buffer_frame = buffer_frames_;
    if (const auto err = audio_.openStream(as_ptr(output_parameters_),
                                           as_ptr(input_parameters_),
                                           format_,
                                           sample_rate_,
                                           &buffer_frames_,
                                           callback_,
                                           nullptr,
                                           &stream_options_))
    {
        throw StreamException{audio_.getErrorText()};
    }

    if (desired_buffer_frame != buffer_frames_)
    {
        throw StreamException{fmt::format("Buffer frame value changed: {} -> {}", desired_buffer_frame, buffer_frames_)};
    }

}

} // namespace audio_producer
