#include <SDL2/SDL.h>
#include "game.h"

int main() {
    // Initialize game window
    Game* game = new Game();
    game->Init();

    // Game loop
    while(!game->config.terminate) {
        game->config.timer.UpdateTimer();
        game->ProcessInput();

        while (game->config.timer.delta >= game->config.ms_per_update) {
            game->Update(game->config.ms_per_update / 1000.0f);
            game->config.timer.delta -= game->config.ms_per_update;
        }
        
        game->GenerateOutput();
    }

    delete game;
    return 0;
}
