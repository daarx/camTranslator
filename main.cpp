#include "Translator.h"
#include "ImageAdjuster.h"

int main() {
    std::string command;

    std::cout << "Type A to adjust camera image, T to enter translation mode: " << std::endl;
    std::cin >> command;

    if (command.starts_with("A") || command.starts_with("a")) {
        camTranslator::ImageAdjuster{}.adjustImage();
    } else if (command.starts_with("T") || command.starts_with("t")) {
        camTranslator::Translator{}.startProgramLoop();
    } else {
        std::cout << "Could not determine command. Exiting." << std::endl;
    }

    // TODO list
    // [ ] Add option to also translate the text

    return 0;
}