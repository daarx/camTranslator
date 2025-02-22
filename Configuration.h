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

public:
    Configuration();

    std::string& getAzureOcrUrl();
    std::string& getAzureOcrPath();
    std::string& getAzureOcrKey();
};

} // camTranslator

#endif //CONFIGURATION_H
