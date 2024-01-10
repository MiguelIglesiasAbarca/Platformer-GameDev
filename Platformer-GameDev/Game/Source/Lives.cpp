#include "Lives.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Lives::Lives() : Entity(EntityType::VIDA)
{
    name.Create("Lives");
}

Lives::~Lives() {}

bool Lives::Awake()
{
    position.x = parameters.attribute("x").as_int();
    position.y = parameters.attribute("y").as_int();
    texturePath = parameters.attribute("texturepath").as_string();

    return true;
}

bool Lives::Start()
{
    
    /*idle.LoadAnimations("corazon", "item");
    idle.speed = 0.16f;*/
    
    // Initialize textures
    texture = app->tex->Load(texturePath);
    pbody = app->physics->CreateCircle(position.x, position.y, 16, bodyType::STATIC);
    pbody->ctype = ColliderType::ITEM;
    pbody->listener = this;
    pickLives_FXid = app->audio->LoadFx("Assets/Audio/Fx/pick_lives_FX.wav");

    return true;
}

bool Lives::Update(float dt)
{
    position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
    position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

    app->render->DrawTexture(texture, position.x, position.y);

    return true;
}

bool Lives::CleanUp()
{
    return true;
}

void Lives::OnCollision(PhysBody* physA, PhysBody* physB)
{
    switch (physB->ctype)
    {
    case ColliderType::PLAYER:
        LOG("Collision PLAYER");
        app->audio->PlayFx(pickLives_FXid, 0);
        pbody->body->SetActive(false);
        app->entityManager->DestroyEntity(this);
        app->physics->world->DestroyBody(pbody->body);
        break;
    }
}
