#include <stdio.h>
#include "utils.h"

void colorprint(ColorFG color_fg, ColorBG color_bg, const char* str) {
    printf("\033[%d;%dm%s\033[0m", color_fg, color_bg, str);
}

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y + 1, x * 2 + 1);
}

DWORD get_current_time_ms() {
    return GetTickCount();
}

void delay_to_maintain_fps(DWORD start, int target_ms) {
    DWORD elapsed = GetTickCount() - start;
    if (elapsed < (DWORD)target_ms) {
        Sleep(target_ms - elapsed);
    }
}
