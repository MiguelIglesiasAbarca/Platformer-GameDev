#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scenes.h"
#include "Map.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

Scenes::Scenes(App* app, bool start_enabled) : Module(app, start_enabled)
{
    name.Create("scenes");
}

// Destructor
Scenes::~Scenes()
{}

// Called before render is available
bool Scenes::Awake(pugi::xml_node& config)
{
    return true;
}

// Called before the first frame
bool Scenes::Start()
{
    // Establecer el estado inicial como INTRO
    currentState = INTRO;

    return true;
}

// Called each loop iteration
bool Scenes::PreUpdate()
{
    return true;
}

bool Scenes::Update(float dt)
{
    switch (currentState)
    {
    case INTRO:
        // Lógica de la introducción aquí

        // Si se presiona una tecla (por ejemplo, la tecla SPACE), cambiar al estado DISPLAY_IMAGE
        if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
        {
            // Cargar la imagen de introducción
            sceneTexture = app->tex->Load("Assets/Textures/intro_image.png");

            // Cambiar al estado DISPLAY_IMAGE
            SetState(DISPLAY_IMAGE);
            LOG("Switching to DISPLAY_IMAGE state");
        }
        break;

    case DISPLAY_IMAGE:
        // Dibujar la escena en la pantalla
        app->render->DrawTexture(sceneTexture, 0, 0, NULL);

        // Si se presiona una tecla (por ejemplo, la tecla P), cambiar al estado IN_GAME
        if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
        {
            // Cargar la imagen del juego
            sceneTexture = app->tex->Load("Assets/Textures/game_image.png");

            // Cambiar al estado IN_GAME
            SetState(IN_GAME);
            LOG("Switching to IN_GAME state");
        }
        break;

    case IN_GAME:
        // Lógica del juego aquí
        LOG("In the IN_GAME state");

        // Si se presiona una tecla (por ejemplo, la tecla R), cambiar al estado DEATH
        if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
        {
            // Cargar la imagen de muerte
            sceneTexture = app->tex->Load("Assets/Textures/death_image.png");

            // Cambiar al estado DEATH
            SetState(DEATH);
            LOG("Switching to DEATH state");
        }
        break;

    case DEATH:
        // Lógica de muerte aquí
        LOG("In the DEATH state");

        // Si se presiona una tecla (por ejemplo, la tecla Enter), cambiar al estado GAME_OVER
        if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
        {
            // Cargar la imagen de fin de juego
            sceneTexture = app->tex->Load("Assets/Textures/game_over_image.png");

            // Cambiar al estado GAME_OVER
            SetState(GAME_OVER);
            LOG("Switching to GAME_OVER state");
        }
        break;

    case GAME_OVER:
        // Lógica de fin de juego aquí
        LOG("In the GAME_OVER state");

        // Si se presiona una tecla (por ejemplo, la tecla N), cambiar al estado LEVEL_CHANGE
        if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
        {
            // Cargar la imagen de cambio de nivel
            sceneTexture = app->tex->Load("Assets/Textures/fondo0.png");
            LOG("Switching to LEVEL_CHANGE state");

            // Cambiar al estado LEVEL_CHANGE
            SetState(LEVEL_CHANGE);
            LOG("Switching to LEVEL_CHANGE state");
        }
        break;

    case LEVEL_CHANGE:
        // Lógica de cambio de nivel aquí
        LOG("In the LEVEL_CHANGE state");

        // Si se presiona una tecla (por ejemplo, la tecla Enter), cambiar al estado LEVEL_COMPLETED
        if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
        {
            // Cargar la imagen de nivel completado
            sceneTexture = app->tex->Load("Assets/Textures/level_completed_image.png");

            // Cambiar al estado LEVEL_COMPLETED
            SetState(LEVEL_COMPLETED);
            LOG("Switching to LEVEL_COMPLETED state");
        }
        break;

    case LEVEL_COMPLETED:
        // Lógica de nivel completado aquí
        LOG("In the LEVEL_COMPLETED state");

        // Si se presiona una tecla (por ejemplo, la tecla H), cambiar al estado HUD
        if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
        {
            // Cargar la imagen del HUD
            sceneTexture = app->tex->Load("Assets/Textures/hud_image.png");

            // Cambiar al estado HUD
            SetState(HUD);
            LOG("Switching to HUD state");
        }
        break;

    case HUD:
        // Lógica del HUD aquí
        LOG("In the HUD state");
        break;
    }

    return true;
}

// Called each loop iteration
bool Scenes::PostUpdate()
{
    // Lógica común para todas las escenas después de la actualización

    return true;
}

bool Scenes::LoadState(pugi::xml_node node)
{
    return true;
}

bool Scenes::SaveState(pugi::xml_node node)
{
    return true;
}

// Called before quitting
bool Scenes::CleanUp()
{
    LOG("Freeing scenes");

    // Liberar la textura de la escena
    if (sceneTexture != nullptr)
    {
        app->tex->UnLoad(sceneTexture);
        sceneTexture = nullptr;
    }

    return true;
}

void Scenes::SetState(SceneState newState)
{
    currentState = newState;
}
