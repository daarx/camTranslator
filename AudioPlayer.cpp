//
// Created by henrik on 27/02/2025.
//

#include "AudioPlayer.h"

#include "raylib_opencv_headers.h"

#include <thread>

camTranslatorAudio::AudioPlayer::AudioPlayer() {
    InitAudioDevice();
}

camTranslatorAudio::AudioPlayer::~AudioPlayer() {
    CloseAudioDevice();
}

void camTranslatorAudio::AudioPlayer::playSound() {
    Sound soundFile = LoadSound("../converted_audio.mp3");

    PlaySound(soundFile);

    while (IsSoundPlaying(soundFile)) {
        // Wait until audio file has finished playing
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    UnloadSound(soundFile);
}
