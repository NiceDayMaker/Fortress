#ifndef SOUND_H
#define SOUND_H

// MiniAudio 헤더 포함
#include "miniaudio.h"

typedef struct {
    ma_sound moveSound;
    ma_sound setSound;
    ma_sound shootSound;
} SoundEffects;

// 초기화 및 해제
void initSoundEngine(); // 오디오 엔진 초기화
void initSounds(SoundEffects* effects);      // 사운드 로드
void freeSounds(SoundEffects* effects);      // 리소스 해제
void shutdownSoundEngine(); // 오디오 엔진 종료

// 사운드 재생 함수
// local
void playMoveSound(SoundEffects* effects);
void stopMoveSound(SoundEffects* effects);
void playSetSound(SoundEffects* effects);
void playShootSound(SoundEffects* effects);
// global
void playExplosionSound();
void playSelectSound();

#endif // SOUND_H
