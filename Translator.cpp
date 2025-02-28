//
// Created by henrik on 23/02/2025.
//

#include "Translator.h"

#include <fstream>
#include <iostream>
#include "json.hpp"

#include "AudioPlayer.h"

namespace camTranslator {
    Translator::Translator() {
        SetConsoleOutputCP(CP_UTF8);
        std::cout << "Initializing configurations..." << std::endl;
        config = std::make_unique<Configuration>();

        std::cout << "Initializing webcam..." << std::endl;
        initializeWebcam();

        std::cout << "Initializing OCR web client..." << std::endl;
        initializeOcrClient();

        std::cout << "Initializing TTS web client..." << std::endl;
        initializeTTSClient();

        std::cout << "Initializing audio player..." << std::endl;
        initializeAudioPlayer();
    }

    Translator::~Translator() {
        capture->release();
    }

    void Translator::startProgramLoop() const {
        bool running = true;

        while (running) {
            char input;

            std::cout << "Press enter to capture an image. Type Q and enter to quit." << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
            input = std::cin.get();

            if (input == 'Q' || input == 'q') {
                running = false;
                continue;
            }

            // capture_image_to_disk_with_webcam(config);
            std::vector<char> image_buffer = loadImageFromDisk();

            std::string extracted_text = extractTextFromImage(image_buffer);
            // std::string extracted_text = "皆さまのことは丁重にもてなすよう大統領閣下より申し付かっております。";

            std::cout << "Extracted text: " << extracted_text << std::endl;

            convertTextToSpeech(extracted_text);

            audioPLayer->playSound();
        }
    }

    void Translator::initializeWebcam() {
        capture = std::make_unique<cv::VideoCapture>(0, cv::VideoCaptureAPIs::CAP_DSHOW);

        if (!capture->isOpened()) {
            throw std::runtime_error("Could not open the camera!");
        }

        capture->set(cv::VideoCaptureProperties::CAP_PROP_FRAME_WIDTH, 3840);
        capture->set(cv::VideoCaptureProperties::CAP_PROP_FRAME_HEIGHT, 2160);
        capture->set(cv::VideoCaptureProperties::CAP_PROP_BRIGHTNESS, config->getCameraBrightness());
        capture->set(cv::VideoCaptureProperties::CAP_PROP_CONTRAST, config->getCameraContrast());
        capture->set(cv::VideoCaptureProperties::CAP_PROP_SHARPNESS, config->getCameraSharpness());
        capture->set(cv::VideoCaptureProperties::CAP_PROP_SATURATION, config->getCameraSaturation());
        capture->set(cv::VideoCaptureProperties::CAP_PROP_FOCUS, config->getCameraFocus());

        double width = capture->get(cv::VideoCaptureProperties::CAP_PROP_FRAME_WIDTH);
        double height = capture->get(cv::VideoCaptureProperties::CAP_PROP_FRAME_HEIGHT);
        std::cout << "Initialized camera with resolution: " << width << "x" << height << std::endl;
    }

    void Translator::initializeOcrClient() {
        ocrClient = std::make_unique<httplib::SSLClient>(config->getAzureOcrUrl());
        ocrClient->set_read_timeout(10, 0);
        ocrClient->set_write_timeout(30, 0);
        ocrClient->set_address_family(AF_INET);
        ocrClient->set_ca_cert_path("../ca-bundle.crt");
        ocrClient->enable_server_certificate_verification(true);
        ocrClient->set_keep_alive(true);

        ocrHeaders = {
            {"Ocp-Apim-Subscription-Key", config->getAzureOcrKey()},
            {"User-Agent", "curl/8.8.0"},
            {"Accept", "*/*"}
        };
    }

    void Translator::initializeTTSClient() {
        ttsClient = std::make_unique<httplib::SSLClient>(config->getAzureSpeechUrl());
        ttsClient->set_read_timeout(10, 0);
        ttsClient->set_write_timeout(30, 0);
        ttsClient->set_address_family(AF_INET);
        ttsClient->set_ca_cert_path("../ca-bundle.crt");
        ttsClient->enable_server_certificate_verification(true);
        ttsClient->set_keep_alive(true);

        ttsHeaders = {
            {"Ocp-Apim-Subscription-Key", config->getAzureSpeechKey()},
            {"User-Agent", "camTranslator"},
            {"Accept", "*/*"},
            {"X-Microsoft-OutputFormat", "audio-16khz-128kbitrate-mono-mp3"}
        };
    }

    void Translator::initializeAudioPlayer() {
        audioPLayer = std::make_unique<camTranslatorAudio::AudioPlayer>();
    }

    void Translator::captureImageToDiskWithWebcam() const {
        cv::Mat frame;

        (*capture) >> frame;

        if (frame.empty()) {
            throw std::runtime_error("Error: Captured frame is empty!");
        }

        if (config->getCropWidth() > 0 && config->getCropHeight() > 0) {
            cv::Rect roi(config->getCropX(), config->getCropY(), config->getCropWidth(), config->getCropHeight());
            frame = frame(roi).clone();
        }

        cv::imwrite("../captured_image.jpg", frame);
        // TODO use imencode to create the image in memory
        std::cout << "Image saved as captured_image.jpg" << std::endl;
    }

    std::string Translator::extractTextFromImage(std::vector<char> &image) const {
        auto res = ocrClient->Post(config->getAzureOcrPath(), ocrHeaders, std::string(image.begin(), image.end()),
                               "application/octet-stream");

        if (res.error() != httplib::Error::Success) {
            std::cout << "Error: " << to_string(res.error()) << std::endl;
            return "Failure";
        }

        std::cout << "Status: " << res->status << std::endl;

        nlohmann::json json = nlohmann::json::parse(res->body);
        std::string extractedText;
        extractedText.reserve(res->body.size());

        if (json.contains("regions")) {
            for (const auto &region: json["regions"]) {
                for (const auto &line: region["lines"]) {
                    for (const auto &word: line["words"]) {
                        extractedText.append(word["text"].get<std::string>());
                    }
                }
            }
        }

        return extractedText;
    }

    void Translator::convertTextToSpeech(std::string text) const {
        std::string body = "<speak version='1.0' xml:lang='ja-JP'><voice xml:lang='ja-JP' xml:gender='Female' name='ja-JP-NanamiNeural'>" + text + "</voice></speak>";

        std::cout << "Sending body: " << body << std::endl;

        auto res = ttsClient->Post(config->getAzureSpeechPath(), ttsHeaders, body, "application/ssml+xml");

        if (res.error() != httplib::Error::Success) {
            std::cout << "Error: " << to_string(res.error()) << std::endl;
            return;
        }

        std::cout << "Status: " << res->status << std::endl;

        std::ofstream file("../converted_audio.mp3", std::ios::binary);
        file.write(res->body.data(), res->body.size());
        file.close();
    }

    std::vector<char> Translator::loadImageFromDisk() {
        std::ifstream file("../test_image.jpg", std::ios_base::in | std::ios_base::binary);

        if (!file.is_open()) {
            throw std::runtime_error("Could not open file!");
        }

        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);

        std::vector<char> buffer(length);

        file.read(buffer.data(), buffer.size());

        file.close();

        std::cout << "Image loaded successfully! Size: " << buffer.size() << std::endl;

        return std::move(buffer);
    }
} // camTranslator
