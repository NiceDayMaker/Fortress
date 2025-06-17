#ifndef INPUT_H
#define INPUT_H

#include <windows.h>
#include <stdbool.h>

typedef enum {
    KEY_LEFT,
    KEY_RIGHT,
    KEY_SHOOT,
    KEY_TOTAL
} GameKey;

typedef struct {
    int key_map[KEY_TOTAL];
    bool prev[KEY_TOTAL];
    bool curr[KEY_TOTAL];
} InputContext;

// 초기화 및 업데이트
void Input_init(InputContext* ctx, const int key_map[KEY_TOTAL]);
void Input_update(InputContext* ctx);

// 상태 쿼리
bool Input_onDown(InputContext* ctx, GameKey key);
bool Input_onHold(InputContext* ctx, GameKey key);
bool Input_onUp(InputContext* ctx, GameKey key);

#endif // INPUT_H
