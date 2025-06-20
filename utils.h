#ifndef UTILS_H
#define UTILS_H

#include <windows.h>

// 전경색
typedef enum {
    COLORFG_BLACK = 30,
    COLORFG_RED,
    COLORFG_GREEN,
    COLORFG_BROWN,
    COLORFG_BLUE,
    COLORFG_MAGENTA,
    COLORFG_CYAN,
    COLORFG_WHITE,
    COLORFG_BRIGHT_BLACK = 90,
    COLORFG_BRIGHT_RED,
    COLORFG_BRIGHT_GREEN,
    COLORFG_BRIGHT_BROWN,
    COLORFG_BRIGHT_BLUE,
    COLORFG_BRIGHT_MAGENTA,
    COLORFG_BRIGHT_CYAN,
    COLORFG_BRIGHT_WHITE
} ColorFG;

// 배경색
typedef enum {
    COLORBG_BLACK = 40,
    COLORBG_RED,
    COLORBG_GREEN,
    COLORBG_BROWN,
    COLORBG_BLUE,
    COLORBG_MAGENTA,
    COLORBG_CYAN,
    COLORBG_WHITE,
    COLORBG_BRIGHT_BLACK = 100,
    COLORBG_BRIGHT_RED,
    COLORBG_BRIGHT_GREEN,
    COLORBG_BRIGHT_BROWN,
    COLORBG_BRIGHT_BLUE,
    COLORBG_BRIGHT_MAGENTA,
    COLORBG_BRIGHT_CYAN,
    COLORBG_BRIGHT_WHITE
} ColorBG;

// 콘솔 제어
void set_color(int r, int g, int b);
void reset_color();
void colorprint(ColorFG color_fg, ColorBG color_bg, const char* str);
void gotoxy(int x, int y);

// 2차원 벡터
typedef struct {
    float x;
    float y;
} Vector2;

// FPS 제어
DWORD get_current_time_ms();
void delay_to_maintain_fps(DWORD frame_start_time, int target_ms);

#endif // UTILS_H
