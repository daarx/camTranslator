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

    double cameraBrightness = 128;
    double cameraContrast = 128;
    double cameraSaturation = 128;
    double cameraSharpness = 128;
    double cameraFocus = 0;

    int cropX = 0;
    int cropY = 0;
    int cropWidth = 0;
    int cropHeight = 0;
    int screenMaxWidth = 0;
    int screenMaxHeight = 0;

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

    double getCameraBrightness() const;
    double getCameraContrast() const;
    double getCameraSaturation() const;
    double getCameraSharpness() const;
    double getCameraFocus() const;

    int getCropX() const;
    int getCropY() const;
    int getCropWidth() const;
    int getCropHeight() const;
    int getScreenMaxWidth() const;
    int getScreenMaxHeight() const;
};

} // camTranslator

#endif //CONFIGURATION_H
