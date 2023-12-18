#include "Final.h"
#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"


Final::Final() : Entity(EntityType::FINAL)
{
    name.Create("Final");
}

Final::~Final() {}

bool Final::Awake()
{
    // Configura la posición y la ruta de la textura según sea necesario
    position.x = parameters.attribute("x").as_int();
    position.y = parameters.attribute("y").as_int();
    texturePath = parameters.attribute("texturepath").as_string();

    return true;
}

bool Final::Start()
{
    // Inicializa la textura
    texture = app->tex->Load(texturePath);

    // Inicializa la animación de estado inicial
    idleAnimation.LoadAnimations("Idle", "Final");
    idleAnimation.speed = 0.167f;

    // Configura la animación actual
    currentAnimation = &idleAnimation;

    return true;
}

bool Final::Update(float dt)
{
    // Verifica si está en contacto con el jugador y cambia el sprite si es necesario
    if (isInContactWithPlayer)
    {
        // Cambia el sprite o realiza acciones específicas al estar en contacto con el jugador
        ChangeSpriteOnContact();
    }

    // Dibuja la textura con la animación actual
    app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());
    currentAnimation->Update();

    return true;
}

bool Final::CleanUp()
{
    // Limpia recursos según sea necesario
    app->tex->UnLoad(texture);

    return true;
}

void Final::OnCollision(PhysBody* physA, PhysBody* physB)
{
    // Maneja colisiones según sea necesario

    // Ejemplo:
    if (physB->ctype == ColliderType::PLAYER)
    {
        isInContactWithPlayer = true;
    }
}

void Final::ChangeSpriteOnContact()
{
    // Implementa el cambio de sprite o acciones adicionales aquí
    // Por ejemplo:
    // currentAnimation = &nuevaAnimacion;
    // texture = app->tex->Load("nueva_textura.png");
}
