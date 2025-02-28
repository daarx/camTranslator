//
// Created by henrik on 23/02/2025.
//

#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <vector>

#include "raylib_opencv_headers.h"

#include "Configuration.h"
#include "AudioPlayer.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

namespace camTranslator {

class Translator {
public:
    Translator();
    ~Translator();
    void startProgramLoop() const;

private:
    std::unique_ptr<Configuration> config;
    std::unique_ptr<cv::VideoCapture> capture;
    std::unique_ptr<httplib::SSLClient> ocrClient;
    std::unique_ptr<httplib::SSLClient> ttsClient;

    httplib::Headers ocrHeaders;
    httplib::Headers ttsHeaders;

    std::unique_ptr<camTranslatorAudio::AudioPlayer> audioPLayer;

    void initializeWebcam();
    void initializeOcrClient();
    void initializeTTSClient();
    void initializeAudioPlayer();

    void captureImageToDiskWithWebcam() const;
    std::string extractTextFromImage(std::vector<char> &image) const;
    void convertTextToSpeech(std::string text) const;
    static std::vector<char> loadImageFromDisk();
};

} // camTranslator

#endif //TRANSLATOR_H
