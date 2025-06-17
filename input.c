#include "input.h"

void Input_init(InputContext* ctx, const int key_map[KEY_TOTAL]) {
    for (int i = 0; i < KEY_TOTAL; ++i) {
        ctx->key_map[i] = key_map[i];
        ctx->prev[i] = false;
        ctx->curr[i] = false;
    }
}

void Input_update(InputContext* ctx) {
    for (int i = 0; i < KEY_TOTAL; ++i) {
        ctx->prev[i] = ctx->curr[i];
        ctx->curr[i] = (GetAsyncKeyState(ctx->key_map[i]) & 0x8000) != 0;
    }
}

bool Input_onDown(InputContext* ctx, GameKey key) {
    return ctx->curr[key] && !ctx->prev[key];
}

bool Input_onHold(InputContext* ctx, GameKey key) {
    return ctx->curr[key];
}

bool Input_onUp(InputContext* ctx, GameKey key) {
    return !ctx->curr[key] && ctx->prev[key];
}
