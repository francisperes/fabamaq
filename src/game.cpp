#include "game.h"

Game::Game() {}
Game::~Game() {
    delete mp_scene;
    delete mp_world;
    delete mp_textRenderer;

    // Clean SDL/Opengl
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(mp_window);
    SDL_Quit();
}

bool Game::Init() {
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	return false;
    }
    // Glew will later ensure that OpenGL 3 is supported on this machine 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Double buffer in GL
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    mp_window = SDL_CreateWindow(
        "Game Development Challenge",
        100,
        100,
        config.window_width,
        config.window_height,
        SDL_WINDOW_OPENGL
    );

    if (mp_window == nullptr) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    atexit(SDL_Quit);

    m_glContext = SDL_GL_CreateContext(mp_window);
    if (!m_glContext) {
        fprintf(stderr, "Failed to create OpenGL context: %s\n", SDL_GetError());
        exit(1);
    }
    
    // glewExperimental = GL_TRUE;
    glewInit();
    if (!glewIsSupported("GL_VERSION_3_3")) {
        fprintf(stderr, "Opengl 3.3 not available");
        exit(1);
    }

    // Get version info 
    const GLubyte* renderer = glGetString(GL_RENDERER); // Get renderer string
    const GLubyte* version = glGetString(GL_VERSION);   // Version as a string 
    SDL_Log("Renderer: %s\n", renderer); 
    SDL_Log("OpenGL version supported %s\n", version); 
    SDL_Log("Renderer: %s\nversion: %s\n", renderer, version); 

    // GL configs
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    LoadScene();
    return true;
}

void Game::ProcessInput() { 
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                config.terminate = true;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_p) {
                    std::cout << "P Key pressed" << '\n';
                    if (!m_roundPlaying) {
                        StartRound();
                    }
                    // Resume game
                    else if (config.timer.paused) { config.timer.ResumeTimer(); }
                    // Pause game
                    else { config.timer.PauseTimer(); }
                }
                if (event.key.keysym.sym == SDLK_i) {
                    m_creditsManager.AddCredit();
                }
                if (event.key.keysym.sym == SDLK_o) {
                    m_creditsManager.TakeOutCredits();
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (m_roundPlaying) {
                    // Swap comments to debug a round while pausing
                    if (!config.timer.paused) EndRound();
                    //EndRound();
                }
        }
    }
}

void Game::Update(float delta) {
    if (m_roundPlaying) {
        mp_world->Update(delta);
        mp_scene->Update(delta);
    }
}

void Game::GenerateOutput() {
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Prepare strings
    std::string credits_in, credits_out, games_played;
    credits_in = "Credits In: " + std::to_string(m_creditsManager.GetCreditsIn());
    credits_out = "Credits Out: " + std::to_string(m_creditsManager.GetCreditsOut());
    games_played = "Games played: " + std::to_string(config.rounds_played);

    // Render scene
    mp_scene->Render(mp_world->GetPositions());

    // Render UI
    mp_textRenderer->Render(credits_in, 10, 560, glm::vec3(1.0f, 1.0f, 1.0f));
    mp_textRenderer->Render(credits_out, 10, 520, glm::vec3(1.0f, 1.0f, 1.0f));
    mp_textRenderer->Render(games_played, 600, 560, glm::vec3(1.0f, 1.0f, 1.0f));
    if (m_roundPlaying) {
        std::string round_objective = "Click on a sphere with the same colour as this string";
        if (m_roundObjective == Objective::RED) {
            mp_textRenderer->Render(round_objective, 10, 10, glm::vec3(1.0f, 0.0f, 0.0f));
        }
        else if (m_roundObjective == Objective::GREEN) {
            mp_textRenderer->Render(round_objective, 10, 10, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else if (m_roundObjective == Objective::BLUE) {
            mp_textRenderer->Render(round_objective, 10, 10, glm::vec3(0.0f, 0.0f, 1.0f));
        }
    }

    SDL_GL_SwapWindow(mp_window);
}

void Game::LoadScene() {
    mp_scene = new renderer::Scene(config.window_width / config.window_height);
    mp_world = new physics::World();
    mp_textRenderer = new renderer::TextRenderer(config.window_width, config.window_height);
}

void Game::StartRound() {
    if (m_creditsManager.GetCreditsIn() <= 0) { 
        std::cout << "Not enough credits" << std::endl;
        return;
    }

    m_creditsManager.UseCredit();
    config.rounds_played++;
    
    int n = rand() % 3;
    if (n == 0) { m_roundObjective = Objective::RED; }
    if (n == 1) { m_roundObjective = Objective::GREEN; }
    if (n == 2) { m_roundObjective = Objective::BLUE; }

    m_roundPlaying = true;
}

void Game::EndRound() {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    physics::Raycast ray = physics::math::RaycastFromCamera(
            mouse_x, mouse_y, mp_scene->GetCameraPointer()->GetPosition(),
            mp_scene->GetCameraPointer()->GetProjectionMatrix(),
            config.window_width, config.window_height
            );

    int index = mp_world->CheckCollision(ray.p, ray.d);
    if (index != -1) {
        std::cout << "HIT! Sphere: " << index << std::endl;
        glm::vec3 c = mp_scene->GetColourFromIndex(index);
        // Check if the correct colour was hit
        if (c.x - 0.001f > 0 && m_roundObjective == Objective::RED) { WinRound(); }
        else if (c.y - 0.001f > 0 && m_roundObjective == Objective::GREEN) { WinRound(); }
        else if (c.z - 0.001f > 0 && m_roundObjective == Objective::BLUE) { WinRound(); }
        else { LoseRound(); std::cout << "Wrong colour" << std::endl; }
    }
    else {
        std::cout << "MISS !" << std::endl; 
        LoseRound();
    }
}

void Game::WinRound() {
    m_creditsManager.AddCredit();
    m_creditsManager.AddCredit();
    m_roundPlaying = false;
}

void Game::LoseRound() {
    m_roundPlaying = false;
}
