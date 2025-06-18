#include <stdio.h>
#include <string.h>
#include "game.h"
#include "render.h"

// 플레이어 추가
static void add_player_impl(GameManager* gm, int id, float x, float y, int facing, float speed, float power, const int key_map[KEY_TOTAL]) {
    if (gm->playerCount >= MAX_PLAYERS) {
        printf("더 이상 플레이어를 추가할 수 없습니다.\n");
        return;
    }

    Player* p = &gm->players[gm->playerCount];
    Player_init(p, id, x, y, facing, speed, power, key_map);
    gm->playerCount++;
}

// 게임 종료 처리
static void gameSet_impl(GameManager* gm, int death_id) {
    if (death_id < 0 || death_id >= gm->playerCount) {
        printf("Error: ID Out of range\n");
        return;
    }
    
    for (int i = 0; i < gm->playerCount; i++) {
        Player* player = &gm->players[i];
        player->isTurn = 0;
        player->fuel = 0;
        player->fire_state = FIRE_STATE_IDLE;
        player->projectile.active = 0;
    }

    render_clear_buffer();

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            render_set_tile(x, y, gm->terrain.get(&gm->terrain, y, x));
        }
    }
    
    for (int i = 0; i < gm->playerCount; i++) {
        if (gm->players[i].health > 0) {
            render_set_player(&gm->players[i]);
        }
    }
    
    render_present();

    gotoxy(11, HEIGHT / 2 - 1);
    colorprint(death_id == 0 ? COLORFG_BRIGHT_RED : COLORFG_BRIGHT_GREEN, COLORBG_BLACK, "┌─────────────────┐");
    gotoxy(11, HEIGHT / 2);
    colorprint(death_id == 0 ? COLORFG_BRIGHT_RED : COLORFG_BRIGHT_GREEN, COLORBG_BLACK, death_id == 0 ? "│      LOSER      │" : "│     WINNER!     │");
    gotoxy(11, HEIGHT / 2 + 1);
    colorprint(death_id == 0 ? COLORFG_BRIGHT_RED : COLORFG_BRIGHT_GREEN, COLORBG_BLACK, "└─────────────────┘");
    reset_color();

    gotoxy(WIDTH - 9, HEIGHT / 2 - 1);
    colorprint(COLORFG_BRIGHT_WHITE, COLORBG_BLACK, "┌──────────────────┐");
    gotoxy(WIDTH - 9, HEIGHT / 2);
    colorprint(COLORFG_BRIGHT_WHITE, COLORBG_BLACK, "│     GAME SET     │");
    gotoxy(WIDTH - 9, HEIGHT / 2 + 1);
    colorprint(COLORFG_BRIGHT_WHITE, COLORBG_BLACK, "└──────────────────┘");
    reset_color();

    gotoxy(WIDTH * 2 - 28, HEIGHT / 2 - 1);
    colorprint(death_id == 1 ? COLORFG_BRIGHT_RED : COLORFG_BRIGHT_GREEN, COLORBG_BLACK, "┌─────────────────┐");
    gotoxy(WIDTH * 2 - 28, HEIGHT / 2);
    colorprint(death_id == 1 ? COLORFG_BRIGHT_RED : COLORFG_BRIGHT_GREEN, COLORBG_BLACK, death_id == 1 ? "│      LOSER      │" : "│     WINNER!     │");
    gotoxy(WIDTH * 2 - 28, HEIGHT / 2 + 1);
    colorprint(death_id == 1 ? COLORFG_BRIGHT_RED : COLORFG_BRIGHT_GREEN, COLORBG_BLACK, "└─────────────────┘");
    reset_color();

    fflush(stdout);

    Sleep(3000);
    gm->gameSet = 1;
}

// 게임 매니저 초기화
void initGameManager(GameManager* gm) {
    memset(gm, 0, sizeof(GameManager));

    gm->addPlayer = add_player_impl;

    Terrain_init(&gm->terrain);

    gotoxy(15, HEIGHT / 2 - 5);
    printf("PLAYER 1");
    gotoxy(15, HEIGHT / 2 - 3);
    printf("┌───────┐");
    gotoxy(15, HEIGHT / 2 - 2);
    printf("│   W   │");
    gotoxy(15, HEIGHT / 2 - 1);
    printf("└───────┘");
    gotoxy(7, HEIGHT / 2);
    printf("┌───────┐");
    gotoxy(7, HEIGHT / 2 + 1);
    printf("│   A   │");
    gotoxy(7, HEIGHT / 2 + 2);
    printf("└───────┘");
    gotoxy(23, HEIGHT / 2);
    printf("┌───────┐");
    gotoxy(23, HEIGHT / 2 + 1);
    printf("│   D   │");
    gotoxy(23, HEIGHT / 2 + 2);
    printf("└───────┘");

    gotoxy(WIDTH * 2 - 23, HEIGHT / 2 - 5);
    printf("PLAYER 2");
    gotoxy(WIDTH * 2 - 23, HEIGHT / 2 - 3);
    printf("┌───────┐");
    gotoxy(WIDTH * 2 - 23, HEIGHT / 2 - 2);
    printf("│ NUM 8 │");
    gotoxy(WIDTH * 2 - 23, HEIGHT / 2 - 1);
    printf("└───────┘");
    gotoxy(WIDTH * 2 - 31, HEIGHT / 2);
    printf("┌───────┐");
    gotoxy(WIDTH * 2 - 31, HEIGHT / 2 + 1);
    printf("│ NUM 4 │");
    gotoxy(WIDTH * 2 - 31, HEIGHT / 2 + 2);
    printf("└───────┘");
    gotoxy(WIDTH * 2 - 15, HEIGHT / 2);
    printf("┌───────┐");
    gotoxy(WIDTH * 2 - 15, HEIGHT / 2 + 1);
    printf("│ NUM 6 │");
    gotoxy(WIDTH * 2 - 15, HEIGHT / 2 + 2);
    printf("└───────┘");

    
    fflush(stdout);

    Sleep(3000);
}

// 게임 매니저 메모리 해제
void shutdownGameManager(GameManager* gm) {
    render_shutdown();
    shutdownSoundEngine();
}

// 게임 루프
void updateGame(GameManager* gm) {
    // 플레이어 업데이트
    for (int i = 0; i < gm->playerCount; i++) {
        Player* player = &gm->players[i];
        player->update(player, &gm->terrain);
    }

    // 게임 종료 조건 확인
    for (int i = 0; i < gm->playerCount; i++) {
        Player* player = &gm->players[i];
        if (player->health <= 0) {
            gameSet_impl(gm, i);
            return;
        }
    }

    // 버퍼 초기화
    render_clear_buffer();

    // 타일 버퍼 쓰기
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            render_set_tile(x, y, gm->terrain.get(&gm->terrain, y, x));
        }
    }
    
    // 플레이어 버퍼 쓰기
    for (int i = 0; i < gm->playerCount; i++) {
        if (gm->players[i].health > 0) {
            render_set_player(&gm->players[i]);
        }
    }
    
    // 렌더링
    render_present();
}

// 게임 턴을 넘기는 함수
static int turn = MAX_PLAYERS - 1;

void passTurn(GameManager* gm){
    turn = (turn + 1) % gm->playerCount;
    Player* p = &gm->players[turn];
    p->onTurn(p);
}
