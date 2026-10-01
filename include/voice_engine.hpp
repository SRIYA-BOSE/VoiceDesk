#ifndef VOICE_ENGINE_HPP
#define VOICE_ENGINE_HPP

#include <string>

struct whisper_context;

class VoiceEngine
{
public:
    explicit VoiceEngine(const std::string& modelPath);
    ~VoiceEngine();

    bool initialize();
    std::string listen();

private:
    std::string modelPath;
    whisper_context* context;

    bool recordAudio(const std::string& filename) const;
    std::string transcribe(const std::string& filename);
};

#endif
