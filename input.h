#ifndef INPUT_H
#define INPUT_H

#include <windows.h>
#include <stdbool.h>

// 키 이벤트
typedef enum {
    KEY_LEFT,
    KEY_RIGHT,
    KEY_SHOOT,
    KEY_TOTAL
} GameKey;

// 입력 컨텍스트 구조체
// 이 구조체는 VK 코드 키 매핑과 현재 및 이전 키 상태를 저장합니다.
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
