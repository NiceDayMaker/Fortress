#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#include "game.h"
#include "render.h"
#include "utils.h"
#include "sound.h"

GameManager gameManager;

int player1_keys[KEY_TOTAL] = { 'A', 'D', 'W' };
int player2_keys[KEY_TOTAL] = { VK_NUMPAD4, VK_NUMPAD6, VK_NUMPAD8 };

int main() {
    GAMESET:

    render_init();
    initSoundEngine();

    //타이틀 이미지 로딩
    FILE *fp = fopen("Resources/asciix4.txt", "rb");
    if (fp == NULL) {
        perror("Missing file: Resources/asciix4.txt");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    char *buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Failed to allocate memory");
        fclose(fp);
        return 1;
    }

    fread(buffer, 1, file_size, fp);
    buffer[file_size] = '\0';  // 문자열 종료

    printf("%s", buffer);

    free(buffer);
    fclose(fp);
    
    gotoxy(WIDTH - 9, HEIGHT - 2);
    printf("┌─────────────────┐");
    gotoxy(WIDTH - 9, HEIGHT - 1);
    printf("│                 │");
    gotoxy(WIDTH - 9, HEIGHT);
    printf("└─────────────────┘");
    fflush(stdout);

    float brightness = 0.2f;
    float direction = 0.05f;

    const int base_r = 100;
    const int base_g = 200;
    const int base_b = 255;

    // 입력 버퍼 비우기
    while (kbhit()) getch();

    // 타이틀 화면에서 시작 버튼을 누를 때까지 대기
    while (1) {
        DWORD start = get_current_time_ms();

        int r = (int)(base_r * brightness);
        int g = (int)(base_g * brightness);
        int b = (int)(base_b * brightness);

        gotoxy(WIDTH - 5, HEIGHT - 1);
        set_color(r, g, b);
        printf("PRESS START");
        reset_color();
        fflush(stdout);

        brightness += direction;
        if (brightness >= 1.0f || brightness <= 0.2f) {
            direction *= -1;
        }

        if(_kbhit()){
            playSelectSound();
            for(int i = 0; i < 10; i++) {
                DWORD start = get_current_time_ms();

                gotoxy(WIDTH - 5, HEIGHT - 1);
                if(i % 2 == 0) {
                    set_color(100, 200, 255);
                } else {
                    set_color(255, 255, 200);
                }
                printf("PRESS START");
                reset_color();
                fflush(stdout);

                delay_to_maintain_fps(start, 50);
            }
            break;
        }

        delay_to_maintain_fps(start, 50);
    }

    printf("\033[2J");
    fflush(stdout);

    // 게임 매니저 초기화
    initGameManager(&gameManager);

    // 플레이어 초기화
    gameManager.addPlayer(&gameManager, 0, 7.8f, HEIGHT / 3, 1, 0.2f, 2.5f, player1_keys);
    gameManager.addPlayer(&gameManager, 1, WIDTH - 8, HEIGHT / 3, -1, 0.2f, 2.5f, player2_keys);

    passTurn(&gameManager);

    // 게임 루프
    while (1)
    {
        DWORD start = get_current_time_ms();

        updateGame(&gameManager);

        // 게임 재시작
        if (gameManager.gameSet) {
            shutdownGameManager(&gameManager);
            goto GAMESET;
        }

        delay_to_maintain_fps(start, 50);
    }

    return 0;
}
