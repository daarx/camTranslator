//
// Created by henrik on 27/02/2025.
//

#include "AudioPlayer.h"

#include <thread>

#include "raylib.h"

void camTranslatorAudio::AudioPlayer::playSound() {
    InitAudioDevice();

    Sound soundFile = LoadSound("../converted_audio.mp3");

    PlaySound(soundFile);

    while (IsSoundPlaying(soundFile)) {
        // Wait until audio file has finished playing
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    UnloadSound(soundFile);
    CloseAudioDevice();
}
