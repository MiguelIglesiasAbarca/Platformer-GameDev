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

	runLeft.LoadAnimations("Bombleft", "cerdoBomba");
	runLeft.speed = 0.16f;
	//dead
	dead.LoadAnimations("Dead", "cerdoBomba");
	dead.speed = 0.25f;
	//watifok
	watifokIn.LoadAnimations("Watifokin", "cerdoBomba");
	watifokIn.speed = 0.05f;

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
	//initialTransform = pbody->body->GetTransform();

	//LoadAnimations();

	return true;
}

bool CerdoPatrullador::Update(float dt)
{

	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	currentVelocity.y += 0.5;

	//health -= 1;
	
	/*if (isFollowingPlayer)
	{
		app->render->DrawTexture(texture, position.x-10, position.y - 10, &watifokIn.GetCurrentFrame());
	}*/
	/*else
	{
		app->render->DrawTexture(texture, position.x, position.y + 1, &watifokIn.GetCurrentFrame());
	}*/

	// Resto de tu código de dibujo...

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

			//watifokIn.Reset();
		}

		if (position.x < app->scene->player->position.x)
		{
			currentVelocity.x = speed * 2.5;
			currentAnimation = &runRight;
			pbody->body->SetLinearVelocity(currentVelocity);
		}
		else if (position.x > app->scene->player->position.x)
		{
			currentVelocity.x = -speed * 2.5;
			currentAnimation = &runLeft;
			pbody->body->SetLinearVelocity(currentVelocity);
		}
	}
	else
	{
		isFollowingPlayer = false;

		if (position.x >= posB)
		{
			direction = false;
			currentAnimation = &runLeft;
		}
		if (position.x <= posA)
		{
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

	if (isDead)
	{
		currentAnimation = &dead;
		currentAnimation->loopCount = 0;
		currentVelocity.x = 0;
		pbody->body->SetLinearVelocity(currentVelocity);
		app->render->DrawTexture(texture, position.x-75, position.y - 124, &currentAnimation->GetCurrentFrame());
		currentAnimation->Update();
	}
	else if(isFollowingPlayer)
	{
		app->render->DrawTexture(texture, position.x, position.y +1, &currentAnimation->GetCurrentFrame());
		currentAnimation->Update();
		currentAnimation = &watifokIn;
		app->render->DrawTexture(texture, position.x + 7, position.y - 5, &currentAnimation->GetCurrentFrame());
		currentAnimation->Update();
	}
	else
	{
		app->render->DrawTexture(texture, position.x, position.y + 1, &currentAnimation->GetCurrentFrame());
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
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::CEILING:
		LOG("Collision CEILING");
		break;
	case ColliderType::TRAP:
		LOG("Collision TRAP");
		break;
	case ColliderType::NEXTLEVEL:
		LOG("Collision NEXTLEVEL");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}