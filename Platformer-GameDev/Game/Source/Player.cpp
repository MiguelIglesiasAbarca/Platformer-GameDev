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

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
    idle.PushBack({ 6, 62, 44, 44 });
    idle.PushBack({ 84, 62, 44, 44 });
    idle.PushBack({ 162, 62, 44, 44 });
    idle.PushBack({ 240, 62, 44, 44 });
    idle.PushBack({ 318, 62, 44, 44 });
    idle.PushBack({ 396, 62, 44, 44 });
    idle.PushBack({ 474, 62, 44, 44 });
    idle.PushBack({ 552, 62, 44, 44 });
    idle.PushBack({ 630, 62, 44, 44 });
    idle.PushBack({ 708, 62, 44, 44 });
    idle.PushBack({ 786, 62, 44, 44 });
    idle.loop = true;
    idle.speed = 0.2f;
    //run
    Runright.PushBack({ 6, 12, 45, 38 });
    Runright.PushBack({ 84, 12, 45, 38 });
    Runright.PushBack({ 162, 12, 45, 38 });
    Runright.PushBack({ 240, 12, 45, 38 });
    Runright.PushBack({ 318, 12, 45, 38 });
    Runright.PushBack({ 396, 12, 45, 38 });
    Runright.PushBack({ 474, 12, 45, 38 });
    Runright.PushBack({ 552, 12, 45, 38 });
    Runright.loop = true;
    Runright.speed = 0.2f;

    Runleft.PushBack({ 6, 114, 55, 35 });
    Runleft.PushBack({ 84, 114, 55, 35 });
    Runleft.PushBack({ 162, 114, 55, 35 });
    Runleft.PushBack({ 240, 114, 55, 35 });
    Runleft.PushBack({ 318, 114, 55, 35 });
    Runleft.PushBack({ 396, 114, 55, 35 });

    Runleft.loop = true;
    Runleft.speed = 0.2f;

    Dead.PushBack({ 6, 157, 49, 46 });
    Dead.PushBack({ 79, 157, 49, 46 });
    Dead.PushBack({ 157, 157, 49, 46 });
    Dead.PushBack({ 235, 157, 49, 46 });
   
    Dead.loop = false;
    Dead.speed = 0.1f;
    Jump.PushBack({ 6, 211, 43, 43 });
    Jump.PushBack({ 74, 211, 43, 43 });
    Jump.loop = false;
    Jump.speed = 0.05f;
    currentAnimation = &idle;
    pbody = app->physics->CreateCircle(position.x, position.y, 12, bodyType::DYNAMIC);
    //pbody= app->physics->CreateRectangle(position.x, position.y, 37, 29, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
    currentAnimation = &idle;
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool godMode = false;

bool Player::Update(float dt)
{
    if (!running && !isDead)
    {
        currentAnimation = &idle;
    }
    
    //if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
    //    // Perform level reset actions here
    //    app->physics->CleanUp(); // Clean up the physics system
    //    app->physics->Init();    // Reinitialize the physics system

    //    // Reset player position and state as needed
    //    position.x = initialX; // Set to the initial X position
    //    position.y = initialY; // Set to the initial Y position
    //    pbody->body->SetTransform(b2Vec2(PIXELS_TO_METERS(position.x + 18), PIXELS_TO_METERS(position.y + 15)), 0);
    //    pbody->body->SetLinearVelocity(b2Vec2(0, 0)); // Reset player velocity
    //    isJumping = false; // Reset jumping state

    //    // You may need to reset other game-specific state and objects here

    //    // After resetting, return to prevent further update during the reset.
    //    return true;
    //}

    b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
    b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

    currentVelocity.y += 0.5;

    //LOG("current velocity: %f", currentVelocity.y);
    // Si no se presionan las teclas de movimiento, aplicar una fricción alta
    if (app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT) {
        currentVelocity.x *= 0.0;
        running = false;
    }

    // Verificar si el jugador está en el suelo (velocidad vertical cercana a cero)
    //bool isOnGround = std::abs(currentVelocity.y) < 0.1;
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

    // Saltar independientemente del "modo dios" si no estamos ya en el aire y en el suelo
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isJumping && !isDead) {
        // Saltar solo si no estamos ya en el aire
        isJumping = true;
        currentVelocity.y = -15;
        pbody->body->SetLinearVelocity(currentVelocity);
    }

    if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !isDead) {
        currentVelocity.x = -speed;
        currentAnimation = &Runleft;
        running = true;
    }

    if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !isDead) {
        currentVelocity.x = speed;
        currentAnimation = &Runright;
        running = true;
    }

    if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
        //currentVelocity.x = speed;
        isDead = true;
        currentAnimation = &Dead;
        running = false;
    }

    if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
        // Cambiar el estado del modo "godmode" al presionar F10
        godMode = !godMode;
    }

    if (godMode) {
        
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
    else {
        // Restaurar la máscara de colisión normal cuando no está en modo "godmode"
        b2Filter filter = pbody->body->GetFixtureList()->GetFilterData();
        filter.maskBits = 0xFFFF;  // Colisiona con todas las capas
        pbody->body->GetFixtureList()->SetFilterData(filter);

        // Si no está en modo "godmode", aplicar la gravedad
        pbody->body->SetGravityScale(1.0f);
        //if(!isJumping && currentVelocity.y != -GRAVITY_Y) currentVelocity.y += 0.1;
        pbody->body->SetLinearVelocity(currentVelocity);
    }

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
    currentAnimation = &Dead;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
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