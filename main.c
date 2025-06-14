#include "terrain.h"
#include "render.h"
#include "player.h"
#include "utils.h"

#include <stdio.h>
#include <windows.h>

int main()
{
    Terrain terrain;
    Terrain_init(&terrain);

    Player player;
    Player_init(&player, 8, 0, 0.2f, 2.0f);

    render_init();

    for (;;)
    {
        DWORD start = get_current_time_ms();

        Player_update(&player, &terrain);

        render_clear_buffer();
        for (int y = 0; y < HEIGHT; y++)
            for (int x = 0; x < WIDTH; x++)
                render_set_tile(x, y, terrain.get(&terrain, y, x));
        render_set_player(&player);
        render_present();

        delay_to_maintain_fps(start, 50);
    }

    render_shutdown();
    return 0;
}
