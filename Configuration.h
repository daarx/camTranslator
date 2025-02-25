//
// Created by henrik on 22/02/2025.
//

#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <string>

namespace camTranslator {

class Configuration {
    std::string azureOcrUrl;
    std::string azureOcrPath;
    std::string azureOcrKey;
    std::string azureSpeechUrl;
    std::string azureSpeechPath;
    std::string azureSpeechKey;
    std::string azureTranslateUrl;
    std::string azureTranslatePath;
    std::string azureTranslateKey;
    std::string azureRegion;

    double cameraBrightness;
    double cameraContrast;
    double cameraSaturation;
    double cameraSharpness;
    double cameraFocus;

public:
    Configuration();

    std::string& getAzureOcrUrl();
    std::string& getAzureOcrPath();
    std::string& getAzureOcrKey();
    std::string& getAzureSpeechUrl();
    std::string& getAzureSpeechPath();
    std::string& getAzureSpeechKey();
    std::string& getAzureTranslateUrl();
    std::string& getAzureTranslatePath();
    std::string& getAzureTranslateKey();
    std::string& getAzureRegion();

    double getCameraBrightness();
    double getCameraContrast();
    double getCameraSaturation();
    double getCameraSharpness();
    double getCameraFocus();
};

} // camTranslator

#endif //CONFIGURATION_H
