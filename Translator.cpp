//
// Created by henrik on 23/02/2025.
//

#include "Translator.h"

#include <fstream>
#include <iostream>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "json.hpp"
#include <opencv2/opencv.hpp>

#include "AudioPlayer.h"

namespace camTranslator {
    void Translator::translate_webcam_to_speech() {
        SetConsoleOutputCP(CP_UTF8);

        Configuration config;

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
            std::vector<char> image_buffer = load_image_from_disk();

            std::string extracted_text = extract_text_from_image(image_buffer, config);
            // std::string extracted_text = "皆さまのことは丁重にもてなすよう大統領閣下より申し付かっております。";

            std::cout << "Extracted text: " << extracted_text << std::endl;

            convert_text_to_speech(extracted_text, config);

            camTranslatorAudio::AudioPlayer::playSound();
        }
    }

    void Translator::capture_image_to_disk_with_webcam(Configuration &config) {
        std::cout << "Opening camera" << std::endl;

        cv::VideoCapture cap{0, cv::VideoCaptureAPIs::CAP_DSHOW};
        if (!cap.isOpened()) {
            throw std::runtime_error("Could not open the camera!");
        }

        cap.set(cv::VideoCaptureProperties::CAP_PROP_FRAME_WIDTH, 3840);
        cap.set(cv::VideoCaptureProperties::CAP_PROP_FRAME_HEIGHT, 2160);
        cap.set(cv::VideoCaptureProperties::CAP_PROP_BRIGHTNESS, config.getCameraBrightness());
        cap.set(cv::VideoCaptureProperties::CAP_PROP_CONTRAST, config.getCameraContrast());
        cap.set(cv::VideoCaptureProperties::CAP_PROP_SHARPNESS, config.getCameraSharpness());
        cap.set(cv::VideoCaptureProperties::CAP_PROP_SATURATION, config.getCameraSaturation());
        cap.set(cv::VideoCaptureProperties::CAP_PROP_FOCUS, config.getCameraFocus());

        cv::Mat frame;

        cap >> frame;

        if (frame.empty()) {
            throw std::runtime_error("Error: Captured frame is empty!");
        }

        if (config.getCropWidth() > 0 && config.getCropHeight() > 0) {
            cv::Rect roi(config.getCropX(), config.getCropY(), config.getCropWidth(), config.getCropHeight());
            frame = frame(roi).clone();
        }

        cv::imwrite("../captured_image.jpg", frame);
        // TODO use imencode to create the image in memory
        std::cout << "Image saved as captured_image.jpg" << std::endl;

        cap.release();
    }

    std::string Translator::extract_text_from_image(std::vector<char> &image, Configuration &config) {
        // curl -v -X GET -H "Ocp-Apim-Subscription-Key: 2caGusEMW9Xox7o2pWQdp4Jp1If2HPXpaubYQDcpbfMj8HVlgvBUJQQJ99BBACi5YpzXJ3w3AAAFACOGdIpl" "https://daarx-autotranslate.cognitiveservices.azure.com/vision/v3.2/models"
        httplib::SSLClient client(config.getAzureOcrUrl());
        client.set_read_timeout(10, 0);
        client.set_write_timeout(30, 0);
        client.set_address_family(AF_INET);
        client.set_ca_cert_path("../ca-bundle.crt");
        client.enable_server_certificate_verification(true);
        // client.set_logger([](const httplib::Request &request, const httplib::Response &response) {
        //     std::cout << "[httplib] " << response.status << std::endl;
        // });
        client.set_keep_alive(true);

        httplib::Headers headers{
            {"Ocp-Apim-Subscription-Key", config.getAzureOcrKey()},
            {"User-Agent", "curl/8.8.0"},
            {"Accept", "*/*"}
        };

        auto res = client.Post(config.getAzureOcrPath(), headers, std::string(image.begin(), image.end()),
                               "application/octet-stream");

        if (res.error() != httplib::Error::Success) {
            std::cout << "Error: " << to_string(res.error()) << std::endl;
            return "Failure";
        }

        // auto res = client.Get(AZURE_MODELS_PATH, headers);
        // if (res) {
        //     std::cout << "Azure Response: " << res->body << std::endl;
        // } else {
        //     std::cerr << "GET Request Failed: " << httplib::to_string(res.error()) << std::endl;
        // }

        // auto result = client.get_openssl_verify_result();
        // if (result) {
        //     std::cout << "error";
        //     std::cout << "verify error: " << X509_verify_cert_error_string(result)    << std::endl;
        // }

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

    void Translator::convert_text_to_speech(std::string text, Configuration &config) {
        // curl -v -X POST -H "Ocp-Apim-Subscription-Key: E3yfGwWSVwn8wrDzu5wOzi6fDRxxKmD2TbifeLk9DOOJd7P6ySUkJQQJ99BBACi5YpzXJ3w3AAAYACOGdNcz" -H "X-Microsoft-OutputFormat: audio-16khz-128kbitrate-mono-mp3" -H "Content-Type: application/ssml+xml" --data "<speak version='1.0' xml:lang='ja-JP'><voice xml:lang='ja-JP' xml:gender='Female' name='ja-JP-NanamiNeural'>皆さまのことは丁重にもてなすよう大統領閣下より申し付かっております。</voice></speak>" "https://northeurope.tts.speech.microsoft.com/cognitiveservices/v1"
        httplib::SSLClient client(config.getAzureSpeechUrl());
        client.set_read_timeout(10, 0);
        client.set_write_timeout(30, 0);
        client.set_address_family(AF_INET);
        client.set_ca_cert_path("../ca-bundle.crt");
        client.enable_server_certificate_verification(true);
        client.set_keep_alive(true);

        httplib::Headers headers{
            {"Ocp-Apim-Subscription-Key", config.getAzureSpeechKey() },
            {"User-Agent", "camTranslator"},
            {"Accept", "*/*"},
            {"X-Microsoft-OutputFormat", "audio-16khz-128kbitrate-mono-mp3"}
        };

        std::string body = "<speak version='1.0' xml:lang='ja-JP'><voice xml:lang='ja-JP' xml:gender='Female' name='ja-JP-NanamiNeural'>" + text + "</voice></speak>";

        std::cout << "Sending body: " << body << std::endl;

        auto res = client.Post(config.getAzureSpeechPath(), headers, body, "application/ssml+xml");

        if (res.error() != httplib::Error::Success) {
            std::cout << "Error: " << to_string(res.error()) << std::endl;
            return;
        }

        std::cout << "Status: " << res->status << std::endl;

        std::ofstream file("../converted_audio.mp3", std::ios::binary);
        file.write(res->body.data(), res->body.size());
        file.close();
    }

    std::vector<char> Translator::load_image_from_disk() {
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

        return buffer;
    }

    void Translator::save_image_to_disk(std::vector<char> image) {
        std::ofstream file("captured_image.jpg", std::ios_base::out | std::ios_base::binary);

        file.write(image.data(), image.size());

        file.close();
    }

    void Translator::play_sound_file() {

    }
} // camTranslator
