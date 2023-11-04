#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
    speed = parameters.attribute("speed").as_float();

	return true;
}

bool Player::Start() {

    //initilize textures
    texture = app->tex->Load(texturePath);

#pragma region LOAD_ANIMATIONS

    //idle
    idleRight.LoadAnimations("Idleright");
    idleRight.speed = 0.2f;

    idleLeft.LoadAnimations("Idleleft");
    idleLeft.speed = 0.2f;

    //run
    runRight.LoadAnimations("Runright");
    runRight.speed = 0.2f;

    runLeft.LoadAnimations("Runleft");
    runLeft.speed = 0.2f;

    //dead
    dead.LoadAnimations("Dead");
    dead.speed = 0.1f;

    //jump
    jumpRight.LoadAnimations("Jumpright");
    jumpRight.speed = 0.05f;

    jumpLeft.LoadAnimations("Jumpleft");
    jumpLeft.speed = 0.05f;

#pragma endregion

    currentAnimation = &idleRight;

    pbody = app->physics->CreateCircle(position.x, position.y, 12, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
    currentAnimation = &idleRight;
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update(float dt)
{
    b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
    b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

    currentVelocity.y += 0.5;

#pragma region DEBUG

    // Verificar si el jugador está en el suelo (velocidad vertical cercana a cero)
    if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
    {
         isDead = false;
         pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(550), PIXEL_TO_METERS(1700)), 0);
         level = 1;
    }

    if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN )
    {
        isDead = false;
        pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(1800), PIXEL_TO_METERS(3000)), 0);
        level = 2;
    }

    if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
    {
        isDead = false;
        if (level == 1)
        {
            pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(550), PIXEL_TO_METERS(1700)), 0);
        }
        else
        {
            pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(1800), PIXEL_TO_METERS(3000)), 0);
        }
    }    
    
    if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
        // Cambiar el estado del modo "godmode" al presionar F10
        godMode = !godMode;
    }   
#pragma endregion

#pragma region MOVEMENT

    if (!running && !isDead && !isJumping)
    {
        if (left_right == true)
        {
            currentAnimation = &idleRight;
        }
        else
        {
            currentAnimation = &idleLeft;
        }
    }

   // Si no se presionan las teclas de movimiento, aplicar una fricción alta
    if (app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT) 
    {
        currentVelocity.x *= 0.0;
        running = false;
    }

    // Saltar independientemente del "modo dios" si no estamos ya en el aire y en el suelo
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isJumping && !isDead)// Saltar solo si no estamos ya en el aire
    {
        isJumping = true;
        currentVelocity.y = -15;
        pbody->body->SetLinearVelocity(currentVelocity);

        if (left_right == true)
        {
            currentAnimation = &jumpRight;
        }
        else
        {
            currentAnimation = &jumpLeft;
        }
        currentAnimation->Reset();
    }

    if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !isDead) 
    {
        currentVelocity.x = -speed;
        running = true;
        left_right = false;

        if (isJumping)
        {
            currentAnimation = &jumpLeft;
        }
        else
        {
            currentAnimation = &runLeft;
        }
    }

    if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !isDead) 
    {
        currentVelocity.x = speed;
        running = true;
        left_right = true;

        if (isJumping)
        {
            currentAnimation = &jumpRight;
        }
        else
        {
            currentAnimation = &runRight;
        }
    }

#pragma endregion

#pragma region GODMODE

    if (godMode) 
    {
        
        // Si está en modo "godmode", establece la máscara de colisión para atravesar todo
        b2Filter filter = pbody->body->GetFixtureList()->GetFilterData();
        filter.maskBits = 0x0000;  // No colisiona con ninguna capa (atraviesa todo)
        pbody->body->GetFixtureList()->SetFilterData(filter);

        currentVelocity.y = 0.0;
        pbody->body->SetLinearVelocity(currentVelocity);
        // Controles para volar en modo "godmode"
        b2Vec2 velocity = b2Vec2(0, 0); // Inicializa la velocidad en cero

        if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
            velocity.y = -speed *2; // Volar hacia arriba
        }
        else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
            velocity.y = speed * 2; // Volar hacia abajo
        }

        if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
            velocity.x = -speed * 2; // Moverse hacia la izquierda
        }
        else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
            velocity.x = speed * 2; // Moverse hacia la derecha
        }
        pbody->body->SetGravityScale(0.0f);
        pbody->body->SetLinearVelocity(velocity);
    }
    else 
    {
        // Restaurar la máscara de colisión normal cuando no está en modo "godmode"
        b2Filter filter = pbody->body->GetFixtureList()->GetFilterData();
        filter.maskBits = 0xFFFF;  // Colisiona con todas las capas
        pbody->body->GetFixtureList()->SetFilterData(filter);

        // Si no está en modo "godmode", aplicar la gravedad
        pbody->body->SetGravityScale(1.0f);
        pbody->body->SetLinearVelocity(currentVelocity);
    }

#pragma endregion

    // Actualizar la posición en píxeles
    position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 18;
    position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15;

    app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());
    currentAnimation->Update();

    return true;
}

bool Player::CleanUp()
{
	return true;
}

void Player::OnDeath()
{
    isDead = true;
    running = false;
    currentAnimation = &dead;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		isJumping = false;
		break;
    case ColliderType::TRAP:
        LOG("Collision TRAP");
        OnDeath();
        break;
    case ColliderType::NEXTLEVEL:
        LOG("Collision NEXTLEVEL");
        level = 2;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}