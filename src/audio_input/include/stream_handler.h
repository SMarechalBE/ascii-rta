#ifndef AUDIO_INPUT_STREAM_HANDLER_H
#define AUDIO_INPUT_STREAM_HANDLER_H

/* AudioInput */
#include "device_handler.h"

/* FMT */
#include "fmt/format.h"

/* STL */
#include <optional>
#include <stdexcept>

namespace ascii_rta::input
{

class StreamException : public std::runtime_error
{
public:
    explicit StreamException(std::string_view msg)
        : std::runtime_error{fmt::format("Stream Exception: {}", msg)}
    {
    }
};

template<typename DataType>
class CallbackHandler
{
public:
    CallbackHandler() = default;
    explicit CallbackHandler(const uint8_t channel_count)
        : channel_count_{channel_count}
    {
    }

    int operator()(void* outputBuffer,
                   void* inputBuffer,
                   unsigned int nFrames,
                   double streamTime,
                   RtAudioStreamStatus status,
                   void* /*userData*/)
    {
        return process(static_cast<DataType*>(outputBuffer),
                       static_cast<const DataType*>(inputBuffer),
                       nFrames,
                       streamTime,
                       status);
    }

    virtual ~CallbackHandler() = default;

protected:
    virtual int process(DataType* output,
                        const DataType* input,
                        uint32_t frame_count,
                        double stream_time,
                        RtAudioStreamStatus status) = 0;

    [[nodiscard]] size_t sampleByteSize() const { return sizeof(DataType) * channel_count_; }

    uint8_t channel_count_{1};
};

struct StreamBuilder;

/**
 * @brief RAII class handling an audio stream full lifecycle open -> start -> close -> stop
 *
 * @note It is constructed from a StreamBuilder which is an implementation of the Builder design pattern
 * @warning Audio format is currently hardcoded to Float32, the class could be adapted later on
 */
class StreamHandler
{
public:
    explicit StreamHandler(const StreamBuilder& builder);

    ~StreamHandler() noexcept;
    void start();
    void stop() noexcept;

private:
    static std::optional<RtAudio::StreamParameters>
    _initParameter(const std::optional<DeviceHandler>& device, uint32_t channel, SampleRate sample_rate);
    void _doOpen();
    void _doStart();

    static constexpr auto format_{RTAUDIO_FLOAT32};

    RtAudio& audio_;
    const SampleRate sample_rate_{};
    std::optional<RtAudio::StreamParameters> output_parameters_{};
    std::optional<RtAudio::StreamParameters> input_parameters_{};
    uint32_t buffer_frames_{};
    RtAudio::StreamOptions stream_options_{{}, 2};
    RtAudioCallback callback_{nullptr};
};

/**
 * @brief Builds an instance of a StreamHandler
 *
 * This is an implementation of the Builder design pattern, making use of Named Parameter Idiom (see
 * https://isocpp.org/wiki/faq/ctors#named-parameter-idiom).
 */
struct StreamBuilder
{
    explicit StreamBuilder(RtAudio& audio)
        : audio_{audio}
    {
    }

    StreamBuilder& inputDevice(const DeviceHandler& device, const uint32_t channel)
    {
        input_device_.emplace(device);
        input_channel_count_ = channel;

        return *this;
    }

    StreamBuilder& outputDevice(const DeviceHandler& device, const uint32_t channel)
    {
        output_device_.emplace(device);
        output_channel_count_ = channel;

        return *this;
    }

    StreamBuilder& sampleRate(const SampleRate sample_rate)
    {
        sample_rate_ = sample_rate;

        return *this;
    }

    StreamBuilder& bufferFrames(const uint32_t buffer_frames)
    {
        buffer_frames_ = buffer_frames;

        return *this;
    }

    StreamBuilder& callback(const RtAudioCallback& callback)
    {
        callback_ = callback;

        return *this;
    }

    StreamBuilder& autoStart()
    {
        auto_start_ = true;

        return *this;
    }

    StreamBuilder& nonInterleaved()
    {
        stream_options_.flags |= RTAUDIO_NONINTERLEAVED;

        return *this;
    }

    StreamBuilder& minimizeLatency()
    {
        stream_options_.flags |= RTAUDIO_MINIMIZE_LATENCY;

        return *this;
    }

    StreamBuilder& hogDevice()
    {
        stream_options_.flags |= RTAUDIO_HOG_DEVICE;

        return *this;
    }

    StreamBuilder& scheduleRealtime()
    {
        stream_options_.flags |= RTAUDIO_SCHEDULE_REALTIME;

        return *this;
    }

    StreamBuilder& alsaUseDefault()
    {
        stream_options_.flags |= RTAUDIO_ALSA_USE_DEFAULT;

        return *this;
    }

    StreamBuilder& numberOfBuffers(const uint32_t count)
    {
        stream_options_.numberOfBuffers = count;

        return *this;
    }

    [[nodiscard]] StreamHandler build() const { return StreamHandler{*this}; }

    RtAudio& audio_;
    std::optional<DeviceHandler> output_device_{};
    uint32_t output_channel_count_{};
    std::optional<DeviceHandler> input_device_{};
    uint32_t input_channel_count_{};
    SampleRate sample_rate_{44100};
    uint32_t buffer_frames_{512u};
    RtAudio::StreamOptions stream_options_{{}, 2};
    RtAudioCallback callback_{[](void*, void*, unsigned, double, unsigned, void*) { return 0; }};
    bool auto_start_{false};
};

} // namespace ascii_rta::input

#endif // AUDIO_INPUT_STREAM_HANDLER_H
