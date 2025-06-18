#include <stdio.h>
#include "utils.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

// ANSI 색상 코드 설정
void set_color(int r, int g, int b) {
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}

// ANSI 색상 코드 초기화
void reset_color() {
    printf("\033[0m");
}

// ANSI 색상 출력
void colorprint(ColorFG color_fg, ColorBG color_bg, const char* str) {
    printf("\033[%d;%dm%s\033[0m", color_fg, color_bg, str);
}

// 커서 위치 설정
void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

// 현재 시간(ms) 반환
DWORD get_current_time_ms() {
    return GetTickCount();
}

// FPS 제어용 지연 함수
void delay_to_maintain_fps(DWORD start, int target_ms) {
    DWORD elapsed = GetTickCount() - start;
    if (elapsed < (DWORD)target_ms) {
        //gotoxy(1, 51);
        //printf("TimeSafe: %dms     ", elapsed);
        Sleep(target_ms - elapsed);
    }
    //else {
    //    gotoxy(1, 51);
    //    printf("OverRun : %dms     ", elapsed);
    //}
}
