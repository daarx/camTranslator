#include <opencv2/opencv.hpp>
#include <iostream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "json.hpp"
#include "Configuration.h"

std::vector<char> load_image_from_disk();
void capture_image_to_disk_with_webcam();
std::string extract_text_from_image(std::vector<char> &image, camTranslator::Configuration &config);
void save_image_to_disk(std::vector<char> image);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    camTranslator::Configuration config;

    std::vector<char> image_buffer = load_image_from_disk();

    std::string extracted_text = extract_text_from_image(image_buffer, config);

    std::cout << "Extracted text: " << extracted_text << std::endl;

    return 0;
}

void capture_image_to_disk_with_webcam() {
    std::cout << "Opening camera" << std::endl;

    cv::VideoCapture cap{0, cv::VideoCaptureAPIs::CAP_DSHOW};
    if (!cap.isOpened()) {
        throw std::runtime_error("Could not open the camera!");
    }

    cap.set(cv::VideoCaptureProperties::CAP_PROP_FRAME_WIDTH, 3840);
    cap.set(cv::VideoCaptureProperties::CAP_PROP_FRAME_HEIGHT, 2160);

    cv::Mat frame;

    cap >> frame;

    if (frame.empty()) {
        throw std::runtime_error("Error: Captured frame is empty!");
    }

    cv::imwrite("../captured_image.jpg", frame);
    std::cout << "Image saved as captured_image.jpg" << std::endl;

    cap.release();
}

std::string extract_text_from_image(std::vector<char> &image, camTranslator::Configuration &config) {
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
        { "Ocp-Apim-Subscription-Key", config.getAzureOcrKey() },
        { "User-Agent", "curl/8.8.0" },
        { "Accept", "*/*" }
    };

    auto res = client.Post(config.getAzureOcrPath(), headers, std::string(image.begin(), image.end()), "application/octet-stream");

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

    if (json.contains("regions")) {
        for (const auto& region : json["regions"]) {
            for (const auto& line : region["lines"]) {
                for (const auto& word : line["words"]) {
                    extractedText += word["text"].get<std::string>() + " ";
                }
                extractedText += "\n";
            }
        }
    }

    return extractedText;
}

std::vector<char> load_image_from_disk() {
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

void save_image_to_disk(std::vector<char> image) {
    std::ofstream file("captured_image.jpg", std::ios_base::out | std::ios_base::binary);

    file.write(image.data(), image.size());

    file.close();
}