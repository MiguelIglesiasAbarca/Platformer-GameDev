#ifndef __FINAL__
#define __FINAL__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Final : public Entity
{
public:

    Final();

    virtual ~Final();

    bool Awake();

    bool Start();

    bool Update(float dt);

    bool CleanUp();

    void OnCollision(PhysBody* physA, PhysBody* physB);

    // Nueva funci�n para cambiar el sprite cuando hay contacto con el jugador
    void ChangeSpriteOnContact();

public:
    const char* texturePath;
    SDL_Texture* texture = NULL;

    // Puedes mantener la posici�n y el cuerpo f�sico si es necesario
    // iPoint cerdoPosition;
    // PhysBody* pbody;

    // Puedes mantener una bandera para controlar si est� en contacto con el jugador
    bool isInContactWithPlayer = false;

    // Animation para el estado inicial
    Animation* currentAnimation = nullptr;
    Animation idleAnimation;
    // Agrega m�s animaciones seg�n sea necesario

    // ... Otras propiedades seg�n sea necesario
};

#endif
