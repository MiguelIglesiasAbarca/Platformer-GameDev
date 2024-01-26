#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "SceneDeath.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"
#include "Food.h"


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
	coin = nullptr;
}

Player::~Player() {}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	width = parameters.attribute("w").as_int();
	height = parameters.attribute("h").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	speed = parameters.attribute("speed").as_float();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

#pragma region LOAD_ANIMATIONS

	//idle
	idleRight.LoadAnimations("Idleright", "player");
	idleRight.speed = 0.48f;

	//run
	runRight.LoadAnimations("Runright", "player");
	runRight.speed = 0.167f;

	//dead
	dead.LoadAnimations("Dead", "player");
	dead.speed = 0.1f;

	//jump
	jumpRight.LoadAnimations("Jumpright", "player");
	jumpRight.speed = 0.05f;

	//attack
	attackRight.LoadAnimations("Attackright", "player");
	attackRight.speed = 0.05f;
	//attackRight.speed = 0.2f;

#pragma endregion

	currentAnimation = &idleRight;

	pbody = app->physics->CreateCircle(position.x, position.y, 12, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	jump_FXid = app->audio->LoadFx("Assets/Audio/Fx/Grunt_player_02.wav");
	death_Fxid = app->audio->LoadFx("Assets/Audio/Fx/player_death_FX.wav");
	attack_FXid = app->audio->LoadFx("Assets/Audio/Fx/attack_FX.wav");
	running_FXid = app->audio->LoadFx("Assets/Audio/Fx/footstep_FX.wav");
	checkpoint_FXid = app->audio->LoadFx("Assets/Audio/Fx/what_a_fuck_FX.wav");

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	LOG("Position: %d", position.x);

	currentVelocity.y += 0.5;
	LOG("pos1: %d", position.x);
	LOG("pos2: %d", position.y);
	if (position.x >=  3024 && position.y >=1250)
	{
		
	}
	if (isDead)
	{
		if (dead.HasFinished())
		{
			Reset();
		}
		currentVelocity = b2Vec2(0, 0);
	}

	if (!running && !isDead && !isJumping && !isAttacking)
	{
		currentAnimation = &idleRight;
	}

	if (destroyBody)
	{
		app->physics->world->DestroyBody(AttackpBody->body);
		destroyBody = false;
	}
#pragma region MOVEMENT

	// Si no se presionan las teclas de movimiento, aplicar una fricción alta
	if (app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
	{
		currentVelocity.x = 0.0;
		running = false;
		if (runningFX == true)
		{
			runningFX = false;
			app->audio->PauseFx(running_FXid);
		}
	}

	// Saltar independientemente del "modo dios" si no estamos ya en el aire y en el suelo
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isJumping && !isDead && !isAttacking)// Saltar solo si no estamos ya en el aire
	{
		currentVelocity.y = -15;
		pbody->body->SetLinearVelocity(currentVelocity);
		if (runningFX == true || isJumping == false)
		{
			runningFX = false;
			isJumping = true;
			app->audio->PauseFx(running_FXid);
		}
		app->audio->PlayFx(jump_FXid, 0);
		currentAnimation = &jumpRight;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !isDead && !isAttacking)
	{
		looksRight = false;
		currentVelocity.x = -speed;
		if (!running)
		{
			running = true;
		}

		if (looksRight == true && isJumping == true || looksRight == true)
		{
			app->audio->PauseFx(running_FXid);
			runningFX = false;
			looksRight = false;
		}

		if (isJumping || isAttacking)
		{
			currentAnimation = &jumpRight;
			app->audio->PauseFx(running_FXid);
		}
		else
		{
			currentAnimation = &runRight;
		}

		if (runningFX == false)
		{
			app->audio->PlayFx(running_FXid, -1);
			runningFX = true;
		}

	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !isDead && !isAttacking)
	{
		looksRight = true;
		currentVelocity.x = speed;
		if (!running)
		{
			running = true;
		}
		if (looksRight == false && isJumping == true || looksRight == false)
		{
			app->audio->PauseFx(running_FXid);
			runningFX = false;
			looksRight = true;
		}

		if (isJumping || isAttacking)
		{
			currentAnimation = &jumpRight;
		}
		else
		{
			currentAnimation = &runRight;
		}

		if (runningFX == false)
		{
			app->audio->PlayFx(running_FXid, -1);
			runningFX = true;
		}
	}

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && !isDead && !isAttacking)
	{
		running = false;
		isAttacking = true;
		destroyBody = true;
		app->audio->PauseFx(running_FXid);
		app->audio->PlayFx(attack_FXid, 0);
		currentAnimation = &attackRight;
		currentAnimation->loopCount = 0;
		if (looksRight)
		{
			AttackpBody = app->physics->CreateCircle(position.x + 55, position.y, 18, bodyType::DYNAMIC);
			AttackpBody->ctype = ColliderType::DAMAGE;
		}
		else
		{
			AttackpBody = app->physics->CreateCircle(position.x - 55 , position.y, 18, bodyType::DYNAMIC);
			AttackpBody->ctype = ColliderType::DAMAGE;
		}
		currentAnimation->Reset();
	}

	if (currentAnimation->HasFinished() && !isDead)
	{
		isAttacking = false;
	}
	else if ((currentAnimation->HasFinished() && isDead))
	{
		isAttacking = false;
		OnDeath();
	}

#pragma endregion

#pragma region DEBUG

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		isDead = false;
		pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(550), PIXEL_TO_METERS(1700)), 0);
		level = 1;
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
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
			velocity.y = -speed * 2; // Volar hacia arriba
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
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	flip = looksRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

	app->render->DrawTexture(texture, position.x - (width/2), position.y - (height/2)+3, &currentAnimation->GetCurrentFrame(), flip);
	currentAnimation->Update();

	return true;
}

bool Player::CleanUp()
{
	return true;
}

void Player::Reset()
{
	isDead = false;
	isAttacking = false;
	vida = 3;
	currentAnimation = &idleRight;
	if (level == 1)
	{
		pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(550), PIXEL_TO_METERS(1700)), 0);
	}
	else
	{
		pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(1800), PIXEL_TO_METERS(3000)), 0);
	}
	dead.Reset();
}

void Player::OnDeath()
{
	app->audio->PlayFx(death_Fxid, 0);
	isDead = true;
	running = false;
	currentAnimation = &dead;
	currentAnimation->loopCount = 0;
	app->scenedeath->Enable();
}

void Player::TakeDamage()
{
	app->audio->PlayFx(death_Fxid, 0);
	vida--;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		foodCounter++;
		vida++;
		LOG("Player now has %d lives", vida);
		break;
	case ColliderType::DAMAGE:
		LOG("Collision DAMAGE");
		if (!isDead)
		{
			if (vida != 0)
			{
				TakeDamage();
			}
			else
			{
				OnDeath();
			}
		}
		else
		{
			Reset();
		}
		break;
		case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		isJumping = false;
		break;
		case ColliderType::TRAP:
		LOG("Collision TRAP");
		OnDeath();
		break;
		case ColliderType::CERDO:
		LOG("Collision TRAP");
		if (!isDead)
		{
			if (vida != 0)
			{
				TakeDamage();
			}
			else
			{
				OnDeath();
			}
		}
		else
		{
			Reset();
		}
		break;
	case ColliderType::NEXTLEVEL:
		LOG("Collision NEXTLEVEL");
		if (level == 1)
		{
			app->audio->PlayFx(checkpoint_FXid, 0);
			level = 2;
		}
	}
}