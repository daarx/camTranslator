//
// Created by henrik on 22/02/2025.
//

#include "Configuration.h"

#include <fstream>
#include <iostream>
#include <vector>

namespace camTranslator {
    Configuration::Configuration() {
        std::ifstream envFile("../.env", std::ios_base::in | std::ios_base::ate);

        if (!envFile.is_open()) {
            throw std::runtime_error("Could not open .env file!");
        }

        std::vector<std::string> envFileContents = std::vector<std::string>();
        envFile.seekg(0, std::ios::beg);

        for (char line[1024]; envFile.getline(line, 1024);) {
            envFileContents.emplace_back(line);
        }

        envFile.close();

        for (std::string &line : envFileContents) {
            int equalsIndex = line.find('=');

            if (equalsIndex == -1) {
                continue;
            }

            std::string key = line.substr(0, equalsIndex);
            std::string value = line.substr(equalsIndex + 1);

            if (key == "AZURE_OCR_URL") {
                azureOcrUrl = value;
            } else if (key == "AZURE_OCR_PATH") {
                azureOcrPath = value;
            } else if (key == "AZURE_OCR_KEY") {
                azureOcrKey = value;
            } else if (key == "AZURE_SPEECH_URL") {
                azureSpeechUrl = value;
            } else if (key == "AZURE_SPEECH_PATH") {
                azureSpeechPath = value;
            } else if (key == "AZURE_SPEECH_KEY") {
                azureSpeechKey = value;
            } else if (key == "AZURE_TRANSLATE_URL") {
                azureTranslateUrl = value;
            } else if (key == "AZURE_TRANSLATE_PATH") {
                azureTranslatePath = value;
            } else if (key == "AZURE_TRANSLATE_KEY") {
                azureTranslateKey = value;
            } else if (key == "AZURE_REGION") {
                azureRegion = value;
            } else if (key == "CAMERA_BRIGHTNESS") {
                cameraBrightness = std::stod(value);
            } else if (key == "CAMERA_CONTRAST") {
                cameraContrast = std::stod(value);
            } else if (key == "CAMERA_SHARPNESS") {
                cameraSharpness = std::stod(value);
            } else if (key == "CAMERA_SATURATION") {
                cameraSaturation = std::stod(value);
            } else if (key == "CAMERA_FOCUS") {
                cameraFocus = std::stod(value);
            }
        }

        std::cout << "OCR Url: " << azureOcrUrl << std::endl;
        std::cout << "OCR Path: " << azureOcrPath << std::endl;
        std::cout << "OCR Key: " << azureOcrKey << std::endl;
        std::cout << "Speech Url: " << azureSpeechUrl << std::endl;
        std::cout << "Speech Path: " << azureSpeechPath << std::endl;
        std::cout << "Speech Key: " << azureSpeechKey << std::endl;
        std::cout << "Translate Url: " << azureTranslatePath << std::endl;
        std::cout << "Translate Path: " << azureTranslateUrl << std::endl;
        std::cout << "Translate Key: " << azureTranslateKey << std::endl;
        std::cout << "Region: " << azureRegion << std::endl;

        std::cout << "Brightness: " << cameraBrightness << std::endl;
        std::cout << "Contrast: " << cameraContrast << std::endl;
        std::cout << "Saturation: " << cameraSaturation << std::endl;
        std::cout << "Sharpness: " << cameraSharpness << std::endl;
        std::cout << "Focus: " << cameraFocus << std::endl;
    }

    std::string& Configuration::getAzureOcrUrl() {
        return azureOcrUrl;
    }

    std::string& Configuration::getAzureOcrPath() {
        return azureOcrPath;
    }

    std::string& Configuration::getAzureOcrKey() {
        return azureOcrKey;
    }

    std::string & Configuration::getAzureSpeechUrl() {
        return azureSpeechUrl;
    }

    std::string & Configuration::getAzureSpeechPath() {
        return azureSpeechPath;
    }

    std::string & Configuration::getAzureSpeechKey() {
        return azureSpeechKey;
    }

    std::string & Configuration::getAzureTranslateUrl() {
        return azureTranslateUrl;
    }

    std::string & Configuration::getAzureTranslatePath() {
        return azureTranslatePath;
    }

    std::string & Configuration::getAzureTranslateKey() {
        return azureTranslateKey;
    }

    std::string & Configuration::getAzureRegion() {
        return azureRegion;
    }

    double Configuration::getCameraBrightness() {
        return cameraBrightness;
    }

    double Configuration::getCameraContrast() {
        return cameraContrast;
    }

    double Configuration::getCameraSaturation() {
        return cameraSaturation;
    }

    double Configuration::getCameraSharpness() {
        return cameraSharpness;
    }

    double Configuration::getCameraFocus() {
        return cameraFocus;
    }
} // camTranslator