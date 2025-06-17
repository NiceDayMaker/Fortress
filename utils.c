#include <stdio.h>
#include "utils.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

void set_color(int r, int g, int b) {
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}

void reset_color() {
    printf("\033[0m");
}

void colorprint(ColorFG color_fg, ColorBG color_bg, const char* str) {
    printf("\033[%d;%dm%s\033[0m", color_fg, color_bg, str);
}

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

DWORD get_current_time_ms() {
    return GetTickCount();
}

void delay_to_maintain_fps(DWORD start, int target_ms) {
    DWORD elapsed = GetTickCount() - start;
    if (elapsed < (DWORD)target_ms) {
        //gotoxy(1, 51);
        //printf("Fine   : %dms     ", elapsed);
        Sleep(target_ms - elapsed);
    }
    //else {
    //    gotoxy(1, 51);
    //    printf("OverRun: %dms     ", elapsed);
    //}
}
