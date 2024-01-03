#ifndef __SCENES_H__
#define __SCENES_H__

#include "Module.h"
#include "Player.h"
#include "Cerdo.h"
#include "Cerdo_bomba.h"
#include "Dragon.h"
#include "Cerdo_Volador.h"
#include "Food.h"
#include "Rey.h"
#include "Item.h"
#include "Trap_01.h"
//#include "Tomate.h"

struct SDL_Texture;

class Scenes : public Module
{
public:

    Scenes();

    // Destructor
    virtual ~Scenes();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called before the first frame
    bool Start();

    // Called before all Updates
    bool PreUpdate();

    // Called each loop iteration
    bool Update(float dt);

    // Called before all Updates
    bool PostUpdate();

    // Called before quitting
    bool CleanUp();

    bool LoadState(pugi::xml_node node);

    //
    bool SaveState(pugi::xml_node node);

public:
    // Enumeración para los estados de la escena
    enum SceneState
    {
        INTRO,
        DISPLAY_IMAGE,  // Estado para escenas que muestran imágenes
        IN_GAME,
        PAUSE,
        DEATH,
        GAME_OVER,
        LEVEL_CHANGE,
        LEVEL_COMPLETED,
        HUD
        // Añadir más estados según sea necesario
    };

    // Método para cambiar el estado de la escena
    void SetState(SceneState newState);

private:
    // Agrega esta línea para declarar la textura de la escena
    SDL_Texture* sceneTexture;

    // Variable para almacenar el estado actual de la escena
    SceneState currentState;

};

#endif // __SCENES_H__
