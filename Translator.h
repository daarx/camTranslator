//
// Created by henrik on 23/02/2025.
//

#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <vector>

#include "Configuration.h"

namespace camTranslator {

class Translator {
public:
    static void translate_webcam_to_speech();

private:
    static void capture_image_to_disk_with_webcam();

    static std::string extract_text_from_image(std::vector<char> &image, Configuration &config);

    static std::vector<char> load_image_from_disk();

    static void save_image_to_disk(std::vector<char> image);
};

} // camTranslator

#endif //TRANSLATOR_H
