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
            }
        }

        std::cout << "Url: " << azureOcrUrl << std::endl;
        std::cout << "Path: " << azureOcrPath << std::endl;
        std::cout << "Key: " << azureOcrKey << std::endl;
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
} // camTranslator