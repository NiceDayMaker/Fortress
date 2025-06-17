#include <stdio.h>
#include <stdlib.h>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "sound.h"

// 사운드 파일 경로
#define PATH_MOVE       "./Resources/move.wav"
#define PATH_SET        "./Resources/set.wav"
#define PATH_EXPLOSION  "./Resources/explosion.wav"
#define PATH_SHOOT      "./Resources/shoot.wav"
#define PATH_SELECT     "./Resources/select.wav"

// 전역 엔진 및 사운드 객체
static ma_engine engine;
static ma_sound explosionSound;
static ma_sound selectSound;

// 초기화
void initSoundEngine() {
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        fprintf(stderr, "Audio engine init failed.\n");
        exit(1);
    }

    if (ma_sound_init_from_file(&engine, PATH_EXPLOSION, 0, NULL, NULL, &explosionSound) != MA_SUCCESS) {
        fprintf(stderr, "Failed to load: %s\n", PATH_EXPLOSION);
    }
    if (ma_sound_init_from_file(&engine, PATH_SELECT, 0, NULL, NULL, &selectSound) != MA_SUCCESS) {
        fprintf(stderr, "Failed to load: %s\n", PATH_SELECT);
    }
}

void initSounds(SoundEffects* effects) {
    if (ma_sound_init_from_file(&engine, PATH_MOVE, 0, NULL, NULL, &effects->moveSound) != MA_SUCCESS) {
        fprintf(stderr, "Failed to load: %s\n", PATH_MOVE);
    } else {
        ma_sound_set_looping(&effects->moveSound, MA_TRUE);
    }
    if (ma_sound_init_from_file(&engine, PATH_SET, 0, NULL, NULL, &effects->setSound) != MA_SUCCESS) {
        fprintf(stderr, "Failed to load: %s\n", PATH_SET);
    }
    if (ma_sound_init_from_file(&engine, PATH_SHOOT, 0, NULL, NULL, &effects->shootSound) != MA_SUCCESS) {
        fprintf(stderr, "Failed to load: %s\n", PATH_SHOOT);
    }
}

// 종료
void freeSounds(SoundEffects* effects) {
    ma_sound_uninit(&effects->moveSound);
    ma_sound_uninit(&effects->setSound);
    ma_sound_uninit(&effects->shootSound);
}

void shutdownSoundEngine() {
    ma_sound_uninit(&explosionSound);
    ma_sound_uninit(&selectSound);
    ma_engine_uninit(&engine);
}

// 재생
// local
void playMoveSound(SoundEffects* effects) {
    if (!ma_sound_is_playing(&effects->moveSound)) {
        ma_sound_start(&effects->moveSound);
    }
}
void stopMoveSound(SoundEffects* effects) {
    ma_sound_stop(&effects->moveSound);
}
void playSetSound(SoundEffects* effects) {
    ma_sound_start(&effects->setSound);
}
void playShootSound(SoundEffects* effects) {
    ma_sound_start(&effects->shootSound);
}
// global
void playSelectSound() {
    ma_sound_start(&selectSound);
}
void playExplosionSound() {
    ma_sound_start(&explosionSound);
}
