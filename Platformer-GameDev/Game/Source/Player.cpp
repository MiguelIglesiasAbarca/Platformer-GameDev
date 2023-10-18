#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
    name.Create("Player");
    godMode = false;
}

Player::~Player() {}

bool Player::Awake()
{
    position.x = parameters.attribute("x").as_int();
    position.y = parameters.attribute("y").as_int();
    texturePath = parameters.attribute("texturepath").as_string();

    return true;
}

bool Player::Start()
{
    texture = app->tex->Load(texturePath);
    pbody = app->physics->CreateCircle(0, 0, 12, bodyType::DYNAMIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::PLAYER;
    pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

    return true;
}

bool Player::Update(float dt)
{
    HandleGodMode();
    HandleMovement(dt);
    UpdatePosition();
    DrawPlayer();

    return true;
}

void Player::CleanUp()
{
    app->tex->UnLoad(texture);
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB)
{
    switch (physB->ctype)
    {
    case ColliderType::ITEM:
        app->audio->PlayFx(pickCoinFxId);
        break;
    case ColliderType::PLATFORM:
        isJumping = false;
        break;
    case ColliderType::UNKNOWN:
        break;
    }
}

void Player::HandleGodMode()
{
    if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
    {
        godMode = !godMode;
        if (godMode)
        {
            // Ajustar las máscaras de colisión para atravesar todo en modo "godmode"
            SetCollisionMask(0x0000);
            pbody->body->SetGravityScale(0.0f);
        }
        else
        {
            // Restaurar las máscaras de colisión normales
            SetCollisionMask(0xFFFF);
            pbody->body->SetGravityScale(1.0f);
        }
    }
}

void Player::HandleMovement(float dt)
{
    b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

    if (!godMode)
    {
        if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isJumping)
        {
            currentVelocity.y = -0.6 * dt;
            isJumping = true;
        }
    }

    if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
    {
        currentVelocity.x = -speed * 2 * dt;
    }
    else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
    {
        currentVelocity.x = speed * 2 * dt;
    }

    if (!godMode)
    {
        // Aplicar la gravedad
        currentVelocity.y = -GRAVITY_Y;
    }

    pbody->body->SetLinearVelocity(currentVelocity);
}

void Player::UpdatePosition()
{
    position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 18;
    position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15;
}

void Player::DrawPlayer()
{
    app->render->DrawTexture(texture, position.x, position.y);
}

void Player::SetCollisionMask(uint16 maskBits)
{
    b2Filter filter = pbody->body->GetFixtureList()->GetFilterData();
    filter.maskBits = maskBits;
    pbody->body->GetFixtureList()->SetFilterData(filter);
}
