#ifndef GAME_H
#define GAME_H

#include <stdlib.h> //rand()
#include <time.h>   //time()
// OpenGL
#include <GL/glew.h>
#include <GL/glu.h>
// SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
// Renderer
#include "renderer/scene.h"
#include "renderer/text_renderer.h"
// Physics
#include "physics/world.h"
// Game logic
#include "credits_manager.h"

enum Objective {RED, GREEN, BLUE};

class Game {
public:
    struct Config {
        // Maybe this should be a class
        struct Timer {
            void UpdateTimer() {
                current_time = SDL_GetTicks();
                elapsed_time = current_time - previous_time;
                previous_time = current_time;
                if (paused) { return; }
                delta += elapsed_time;
            }
            void PauseTimer() {
                paused = true;
                paused_delta = delta;
                delta = 0;
                std::cout << "Pausing... " << '\n';
            }
            void ResumeTimer() {
                paused = false;
                delta = paused_delta;
                paused_delta = 0;
                std::cout << "Resuming... " << '\n';
            }
            unsigned int current_time = SDL_GetTicks();
            unsigned int previous_time = current_time;
            unsigned int elapsed_time = 0;
            unsigned int delta = 0;
            unsigned int paused_delta = 0;
            bool paused = false;
        }timer;

        unsigned int ms_per_update = 8;
        unsigned int window_width = 800;
        unsigned int window_height = 600;
        unsigned int rounds_played = 0;
        bool playing_round = false;
        bool terminate = false;
    }config;

    Game();
    ~Game();

    bool Init();

    void ProcessInput();
    void Update(float delta);
    void GenerateOutput();

private:
    void LoadScene();

    void StartRound();
    void EndRound();
    void WinRound();
    void LoseRound();

    SDL_Window* mp_window;
    SDL_GLContext m_glContext;
    renderer::Scene* mp_scene;
    renderer::TextRenderer* mp_textRenderer;
    physics::World* mp_world;

    CreditsManager m_creditsManager;

    unsigned int VAO, VBO;

    // Game round data
    bool m_roundPlaying;
    Objective m_roundObjective;
};

#endif
