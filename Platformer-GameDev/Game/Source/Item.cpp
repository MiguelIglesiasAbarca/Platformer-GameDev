#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Item::Item() : Entity(EntityType::ITEM)
{
    name.Create("item");
}

Item::~Item() {}

bool Item::Awake()
{
    position.x = parameters.attribute("x").as_int();
    position.y = parameters.attribute("y").as_int();
    texturePath = parameters.attribute("texturepath").as_string();

    return true;
}

bool Item::Start()
{
    // Initialize textures
    texture = app->tex->Load(texturePath);
    pbody = app->physics->CreateCircle(position.x, position.y, 16, bodyType::DYNAMIC);
    pbody->ctype = ColliderType::ITEM;
    pbody->listener = this;  // Asigna el item como oyente para las colisiones
    // Puedes agregar m�s inicializaciones seg�n sea necesario

    return true;
}

bool Item::Update(float dt)
{
    position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
    position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

    app->render->DrawTexture(texture, position.x, position.y);

    return true;
}

bool Item::CleanUp()
{
    return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB)
{
    switch (physB->ctype)
    {
    case ColliderType::PLAYER:
        LOG("Collision PLAYER");
        // Agrega aqu� la l�gica para incrementar la vida del jugador
        //app->player->vida += 1;

        // Registra que el jugador ha obtenido una vida adicional
        //LOG("Player has gained an extra life. Current lives: %d", app->player->vida);

        // Puedes agregar efectos de sonido u otras acciones aqu�
        //app->audio->PlayFx(yourItemPickup_FXid, 0);

        // Desactiva el cuerpo f�sico y elimina el �tem
        pbody->body->SetActive(false);
        app->entityManager->DestroyEntity(this);
        app->physics->world->DestroyBody(pbody->body);
        break;
    }
    LOG("una vida masssssssssssssssssssssssssssss");

}

