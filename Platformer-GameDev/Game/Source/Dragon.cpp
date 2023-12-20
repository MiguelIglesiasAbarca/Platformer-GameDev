#include "Dragon.h"
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

Dragon::Dragon() : Entity(EntityType::DRAGON)
{
	name.Create("Dragon");
}

Dragon::~Dragon() {}

bool Dragon::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	speed = parameters.attribute("speed").as_float();

	return true;
}

bool Dragon::Start() {


	//run
	idleRight.LoadAnimations("Idleright", "dragon");
	idleRight.speed = 0.16f;

	dead.LoadAnimations("Dead", "dragon");
	dead.speed = 0.167f;

	currentAnimation = &idleRight;

	//initilize textures
	texture = app->tex->Load(texturePath);
	pathTexture = app->tex->Load("Assets/Textures/tomate.png");
	pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::KINEMATIC); // porque no funciona ?? 
	pbody->ctype = ColliderType::CERDO;
	pbody->listener = this;

	return true;
}

bool Dragon::Update(float dt)
{

	playerTilePos = app->map->WorldToMap(app->scene->player->position.x + 16, app->scene->player->position.y);
	enemyPosition = app->map->WorldToMap(position.x + 8, position.y);

	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	distance = playerTilePos.DistanceTo(enemyPosition);

	if (isDead)
	{
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}

	if (distance < 4)
	{
		currentVelocity.x = 0;
		currentVelocity.y = 0;
		pbody->body->SetLinearVelocity(currentVelocity);
	}
	else if (distance >= 4 && distance <= 10)
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
				currentVelocity.x = -speed;
				currentVelocity.y = 0;
				pbody->body->SetLinearVelocity(currentVelocity);
			}
			else if (path->At(1)->x > positionTilesx && path->At(1)->x != positionTilesx)
			{
				looksRight = true;
				currentVelocity.x = speed;
				currentVelocity.y = 0;
				pbody->body->SetLinearVelocity(currentVelocity);

			}
			else if (path->At(1)->y > positionTilesy)
			{
				currentVelocity.y = speed;
				currentVelocity.x = 0;
				pbody->body->SetLinearVelocity(currentVelocity);
			}
			else if (path->At(1)->y < positionTilesy)
			{
				currentVelocity.y = -speed;
				currentVelocity.x = 0;
				pbody->body->SetLinearVelocity(currentVelocity);
			}
		}
	}
	else if (distance > 10)
	{
		currentVelocity.x = 0;
		currentVelocity.y = 0;
		pbody->body->SetLinearVelocity(currentVelocity);
		app->map->pathfinding->ClearLastPath();
	}

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 18;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	flip = looksRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

	app->render->DrawTexture(texture, position.x-10, position.y - 2, &currentAnimation->GetCurrentFrame(), flip);
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

bool Dragon::CleanUp()
{
	return true;
}

void Dragon::OnDeath()
{
	currentAnimation = &dead;
	currentAnimation->loopCount = 0;
	app->entityManager->DestroyEntity(this);
	app->physics->world->DestroyBody(pbody->body);
}

void Dragon::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::DAMAGE:
		LOG("Collision DAMAGE");
		isDead = true;
		break;
	}

}