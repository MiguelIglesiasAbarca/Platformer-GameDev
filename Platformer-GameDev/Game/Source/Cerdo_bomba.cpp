#include "Cerdo_bomba.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "EntityManager.h"
#include "Map.h"

CerdoPatrullador::CerdoPatrullador() : Entity(EntityType::CERDO_PATRULLADOR)
{
	name.Create("CerdoPatrullador");
}

CerdoPatrullador::~CerdoPatrullador() {}

bool CerdoPatrullador::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	speed = parameters.attribute("speed").as_float();

	return true;
}

bool CerdoPatrullador::Start() {


	//run
	runRight.LoadAnimations("Bombright", "cerdoBomba");
	runRight.speed = 0.16f;

	//dead
	dead.LoadAnimations("Dead", "cerdoBomba");
	dead.speed = 0.25f;
	//watifok
	watifokIn.LoadAnimations("Watifokin", "cerdoBomba");
	watifokIn.speed = 0.16f;

	currentAnimation = &runRight;

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x, position.y, 10, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::CERDO;
	pbody->listener = this;
	posA = position.x - 50;
	posB = position.x + 50;

	pigExplosion_FXid = app->audio->LoadFx("Assets/Audio/Fx/pig_explosion_FX.wav");
	pigOink_FXid = app->audio->LoadFx("Assets/Audio/Fx/pig_oink_FX.wav");

	return true;
}

bool CerdoPatrullador::Update(float dt)
{
	if (tp)
	{
		pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
		tp = false;
	}
	if (!tp)
	{
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);
	}

	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	currentVelocity.y += 0.5;

	
	if (isFollowingPlayer)
	{
		currentAnimation = &watifokIn;
	}

	if (currentAnimation->HasFinished() && isDead)
	{
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	
	}

	if (health <= 0)
	{
		OnDeath();
	}
	
	if (posA - 400 <= app->scene->player->position.x && app->scene->player->position.x <= posB + 400 && app->scene->player->position.y < position.y && app->scene->player->position.y >= position.y - 32)
	{
		if (isFollowingPlayer == false)
		{
			app->audio->PlayFx(pigOink_FXid, 0);
			isFollowingPlayer = true;
		}

		if (position.x < app->scene->player->position.x)
		{
			looksRight = true;
			currentVelocity.x = speed * 2.5;
			currentAnimation = &watifokIn;
			pbody->body->SetLinearVelocity(currentVelocity);
		}
		else if (position.x > app->scene->player->position.x)
		{
			looksRight = false;
			currentVelocity.x = -speed * 2.5;
			currentAnimation = &watifokIn;
			pbody->body->SetLinearVelocity(currentVelocity);
		}
	}
	else
	{
		isFollowingPlayer = false;

		if (position.x >= posB)
		{
			looksRight = false;
			direction = false;
			currentAnimation = &runRight;
		}
		if (position.x <= posA)
		{
			looksRight = true;
			direction = true;
			currentAnimation = &runRight;
		}

		if (direction == false)
		{
			currentVelocity.x = -speed;
			pbody->body->SetLinearVelocity(currentVelocity);
		}
		else
		{
			currentVelocity.x = speed;
			pbody->body->SetLinearVelocity(currentVelocity);
		}
	}

	

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 18;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15;

	SDL_RendererFlip flip = SDL_FLIP_NONE;// Variable para controlar el la orientacion de la textura

	flip = looksRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;// Se establece el volteo dependiendo de la condici�n looksRight
	// Si looksRight es verdadero, se aplica un volteo horizontal; de lo contrario, no se aplica volteo (SDL_FLIP_NONE)

	if (isDead)
	{
		currentAnimation = &dead;
		currentAnimation->loopCount = 0;
		currentVelocity.x = 0;
		pbody->body->SetLinearVelocity(currentVelocity);
		app->render->DrawTexture(texture, position.x-75, position.y - 104, &currentAnimation->GetCurrentFrame(),flip);
		currentAnimation->Update();
	}
	else
	{
		app->render->DrawTexture(texture, position.x, position.y - 12, &currentAnimation->GetCurrentFrame(),flip);
		currentAnimation->Update();
	}

	return true;
}

bool CerdoPatrullador::CleanUp()
{
	return true;
}

void CerdoPatrullador::OnDeath()
{
	isDead = true;
}

void CerdoPatrullador::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		app->audio->PlayFx(pigExplosion_FXid, 0);
		OnDeath(); // Call the OnDeath function
		break;
	case ColliderType::DAMAGE:// Si el tipo de colisionador es DAMAGE (da�o)
		LOG("Collision DAMAGE");
		app->audio->PlayFx(pigExplosion_FXid, 0);
		OnDeath();// Marca al cerdo como muerto
		break;
	}

}