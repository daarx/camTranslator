#include "Translator.h"
#include "ImageAdjuster.h"

int main() {
    std::string command;

    std::cout << "Type A to adjust camera image, T to enter translation mode: " << std::endl;
    std::cin >> command;

    if (command.starts_with("A") || command.starts_with("a")) {
        camTranslator::ImageAdjuster{}.adjustImage();
    } else if (command.starts_with("T") || command.starts_with("t")) {
        camTranslator::Translator::translate_webcam_to_speech();
    } else {
        std::cout << "Could not determine command. Exiting." << std::endl;
    }

    // TODO list
    // [X] Command line arguments to switch between applications
    // [X] Command line arguments and loop to initiate camera-ocr-speech process
    // [ ] Store camera and clients as member variables for reuse
    // [X] Deal with pictures that are too large to fit on the screen
    // [X] Playback audio file
    // [ ] Add option to also translate the text
    // [X] Make it possible to crop out the name of the person in the picture
    // [X] Adjuster output should be copy-pasteable into .env file

    return 0;
}