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

	pbody = app->physics->CreateCircle(0, 0, 12, bodyType::DYNAMIC);
    //pbody= app->physics->CreateRectangle(position.x, position.y, 37, 29, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool godMode = false;

bool Player::Update(float dt)
{
    // Ajustar las máscaras de colisión aquí
    if (godMode) {
        // Si está en modo "godmode", establece la máscara de colisión para atravesar todo
        b2Filter filter = pbody->body->GetFixtureList()->GetFilterData();
        filter.maskBits = 0x0000;  // No colisiona con ninguna capa (atraviesa todo)
        pbody->body->GetFixtureList()->SetFilterData(filter);
    }
    else {
        // Restaurar la máscara de colisión normal cuando no está en modo "godmode"
        b2Filter filter = pbody->body->GetFixtureList()->GetFilterData();
        filter.maskBits = 0xFFFF;  // Colisiona con todas las capas
        pbody->body->GetFixtureList()->SetFilterData(filter);
    }
    
    b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
    b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();



    // Si no se presionan las teclas de movimiento, aplicar una fricción alta
    if (app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT) {
        currentVelocity.x *= 0.0; // Ajusta este valor según tus necesidades
    }

    // Verificar si el jugador está en el suelo (velocidad vertical cercana a cero)
    bool isOnGround = std::abs(currentVelocity.y) < 0.1;

    // Saltar independientemente del "modo dios" si no estamos ya en el aire y en el suelo
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isJumping) {
        // Saltar solo si no estamos ya en el aire
        isJumping = true;
        currentVelocity.y = -0.6 * dt;
        pbody->body->SetLinearVelocity(currentVelocity);
    }

    if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
        currentVelocity.x = -speed* 2 * dt;
    }

    if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
        currentVelocity.x = speed *2 * dt;
    }

    if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
        // Cambiar el estado del modo "godmode" al presionar F10
        godMode = !godMode;
    }

    if (godMode) {
        
        currentVelocity.y = 0.0;
    pbody->body->SetLinearVelocity(currentVelocity);
        // Controles para volar en modo "godmode"
        b2Vec2 velocity = b2Vec2(0, 0); // Inicializa la velocidad en cero

        if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
            velocity.y = -speed * dt; // Volar hacia arriba
        }
        else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
            velocity.y = speed * dt; // Volar hacia abajo
        }

        if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
            velocity.x = -speed * dt; // Moverse hacia la izquierda
        }
        else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
            velocity.x = speed * dt; // Moverse hacia la derecha
        }
        pbody->body->SetGravityScale(0.0f);
        pbody->body->SetLinearVelocity(velocity);
    }
    else {
        // Si no está en modo "godmode", aplicar la gravedad
        pbody->body->SetGravityScale(1.0f);
        if(!isJumping) currentVelocity.y = -GRAVITY_Y;
        pbody->body->SetLinearVelocity(currentVelocity);
    }

    // Actualizar la posición en píxeles
    position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 18;
    position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15;

    app->render->DrawTexture(texture, position.x, position.y);

    return true;
}


bool Player::CleanUp()
{

	return true;
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
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}