#include "voice_engine.hpp"

#include "whisper.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace
{
    struct WavHeader
    {
        char riff[4];
        uint32_t fileSize;
        char wave[4];

        char fmt[4];
        uint32_t fmtSize;
        uint16_t audioFormat;
        uint16_t channels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;

        char data[4];
        uint32_t dataSize;
    };

    std::string trim(const std::string& value)
    {
        const auto first = value.find_first_not_of(" \t\n\r");

        if (first == std::string::npos)
        {
            return "";
        }

        const auto last = value.find_last_not_of(" \t\n\r");

        return value.substr(first, last - first + 1);
    }
}

VoiceEngine::VoiceEngine(const std::string& modelPath)
    : modelPath(modelPath),
      context(nullptr)
{
}

VoiceEngine::~VoiceEngine()
{
    if (context != nullptr)
    {
        whisper_free(context);
        context = nullptr;
    }
}

bool VoiceEngine::initialize()
{
    whisper_context_params params =
        whisper_context_default_params();

    context =
        whisper_init_from_file_with_params(
            modelPath.c_str(),
            params);

    if (context == nullptr)
    {
        std::cerr
            << "VoiceEngine: Failed to load Whisper model: "
            << modelPath
            << '\n';

        return false;
    }

    std::cout
        << "VoiceEngine: Whisper model loaded successfully.\n";

    return true;
}

bool VoiceEngine::recordAudio(const std::string& filename) const
{
    std::cout << "\nListening for 5 seconds...\n";
    std::cout << "Speak a VoiceDesk command now.\n";

    std::string command =
        "timeout 5s parecord "
        "--device=RDPSource "
        "--format=s16le "
        "--rate=16000 "
        "--channels=1 "
        "--file-format=wav "
        "\"" + filename + "\"";

    const int result = std::system(command.c_str());

    // timeout normally returns 124 after stopping parecord.
    if (result != 0 && result != 31744)
    {
        std::cerr
            << "VoiceEngine: Microphone recording failed.\n";

        return false;
    }

    return true;
}

std::string VoiceEngine::transcribe(
    const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        std::cerr
            << "VoiceEngine: Unable to open recorded audio.\n";

        return "";
    }

    WavHeader header{};

    file.read(
        reinterpret_cast<char*>(&header),
        sizeof(header));

    if (!file)
    {
        std::cerr
            << "VoiceEngine: Invalid WAV file.\n";

        return "";
    }

    if (header.audioFormat != 1 ||
        header.channels != 1 ||
        header.sampleRate != 16000 ||
        header.bitsPerSample != 16)
    {
        std::cerr
            << "VoiceEngine: Unsupported audio format.\n";

        return "";
    }

    std::vector<int16_t> pcm16(
        header.dataSize / sizeof(int16_t));

    file.read(
        reinterpret_cast<char*>(pcm16.data()),
        header.dataSize);

    if (!file)
    {
        std::cerr
            << "VoiceEngine: Failed reading audio samples.\n";

        return "";
    }

    std::vector<float> pcmf32(pcm16.size());

    std::transform(
        pcm16.begin(),
        pcm16.end(),
        pcmf32.begin(),
        [](int16_t sample)
        {
            return static_cast<float>(sample) / 32768.0f;
        });

    whisper_full_params params =
        whisper_full_default_params(
            WHISPER_SAMPLING_GREEDY);

    params.print_progress = false;
    params.print_realtime = false;
    params.print_timestamps = false;
    params.print_special = false;

    params.language = "en";
    params.translate = false;

    if (whisper_full(
            context,
            params,
            pcmf32.data(),
            static_cast<int>(pcmf32.size())) != 0)
    {
        std::cerr
            << "VoiceEngine: Whisper transcription failed.\n";

        return "";
    }

    std::string text;

    const int segmentCount =
        whisper_full_n_segments(context);

    for (int i = 0; i < segmentCount; ++i)
    {
        const char* segment =
            whisper_full_get_segment_text(context, i);

        if (segment != nullptr)
        {
            text += segment;
        }
    }

    return trim(text);
}

std::string VoiceEngine::listen()
{
    const std::string filename =
        "/tmp/voicedesk_command.wav";

    if (!recordAudio(filename))
    {
        return "";
    }

    std::cout << "VoiceEngine: Processing speech...\n";

    std::string result =
        transcribe(filename);

    std::remove(filename.c_str());

    return result;
}
