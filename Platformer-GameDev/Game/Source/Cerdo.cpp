#include "Cerdo.h"
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

Cerdo::Cerdo() : Entity(EntityType::CERDO)
{
	name.Create("Cerdo");
}

Cerdo::~Cerdo() {}

bool Cerdo::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	speed = parameters.attribute("speed").as_float();

	return true;
}

bool Cerdo::Start() {

	runRight.LoadAnimations("Runright", "cerdo");
	runRight.speed = 0.167f;

	idleRight.LoadAnimations("Idleright", "cerdo");
	idleRight.speed = 0.167f;

	jumpRight.LoadAnimations("Jumpright", "cerdo");
	jumpRight.speed = 0.167f;

	dead.LoadAnimations("Dead", "cerdo");
	dead.speed = 0.1f;

	attack.LoadAnimations("Attackright", "cerdo");
	attack.speed = 0.01f;

	pathTexture = app->tex->Load("Assets/Textures/tomate.png");
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x, position.y, 10, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::CERDO;
	pbody->listener = this;

	currentAnimation = &idleRight;

	return true;
}

bool Cerdo::Update(float dt)
{
	playerTilePos = app->map->WorldToMap(app->scene->player->position.x + 16, app->scene->player->position.y);
	enemyPosition = app->map->WorldToMap(position.x + 8, position.y);

	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	currentVelocity.y += 0.5;

	distance = playerTilePos.DistanceTo(enemyPosition);

	if (isDead)
	{
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}

	if (distance < 2)
	{
		currentAnimation = &idleRight;
		currentVelocity.x = 0;
		currentVelocity.y += 0.5;
		pbody->body->SetLinearVelocity(currentVelocity);
	}
	else if (distance >= 2 && distance <= 5)
	{
		app->map->pathfinding->CreatePath(enemyPosition, playerTilePos);
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
		if (path->Count() > 1)
		{
			nextTilePath = { path->At(1)->x, path->At(1)->y };

			int positionTilesx = position.x / 32;
			int positionTilesy = position.y / 32;
			if (path->At(1)->x < positionTilesx && path->At(1)->x != positionTilesx)
			{
				looksRight = false;
				currentAnimation = &runRight;
				currentVelocity.x = -speed*3;
				currentVelocity.y += 0.5;
				pbody->body->SetLinearVelocity(currentVelocity);
			}
			else if (path->At(1)->x > positionTilesx && path->At(1)->x != positionTilesx)
			{
				looksRight = true;
				currentVelocity.x = speed*3;
				currentAnimation = &runRight;
				currentVelocity.y += 0.5;
				pbody->body->SetLinearVelocity(currentVelocity);

			}
			else if (path->At(1)->y < positionTilesy && path->At(1)->y != positionTilesy)
			{
				if (!isJumping)
				{
					isJumping = true;
					currentAnimation = &jumpRight;
					currentVelocity.y =  -15;
					pbody->body->SetLinearVelocity(currentVelocity);
				}
			}
		}
	}
	else if (distance > 5)
	{
		isJumping = false;
		currentVelocity.x = 0;
		currentVelocity.y += 0.5;
		currentAnimation = &idleRight;
		pbody->body->SetLinearVelocity(currentVelocity);
		app->map->pathfinding->ClearLastPath();
	}

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 18;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	flip = looksRight ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	app->render->DrawTexture(texture, position.x - 10, position.y - 2, &currentAnimation->GetCurrentFrame(), flip);
	currentAnimation->Update();

	if (app->physics->debug)
	{
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(pathTexture, pos.x + 8, pos.y + 8);
		}
	}

	return true;
}

bool Cerdo::CleanUp()
{
	return true;
}

void Cerdo::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::DAMAGE:
		LOG("Collision DAMAGE");
		isDead = true;
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		isJumping = false;
		break;
	}

}
