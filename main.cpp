#include "Translator.h"
#include "ImageAdjuster.h"

int main() {
    camTranslator::ImageAdjuster{}.adjustImage();
    // camTranslator::Translator::translate_webcam_to_speech();

    // TODO list
    // [ ] Command line arguments to switch between applications
    // [ ] Command line arguments and loop to initiate camera-ocr-speech process
    // [ ] Store camera and clients as member variables for reuse
    // [ ] Deal with pictures that are too large to fit on the screen
    // [ ] Playback audio file
    // [ ] Add option to also translate the text
    // [ ] Make it possible to crop out the name of the person in the picture
    // [X] Adjuster output should be copy-pasteable into .env file

    return 0;
}