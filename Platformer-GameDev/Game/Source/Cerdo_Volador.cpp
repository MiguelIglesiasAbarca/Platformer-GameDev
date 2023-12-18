#include "Cerdo_Volador.h"
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

Cerdo_Volador::Cerdo_Volador() : Entity(EntityType::CERDO_VOLADOR)
{
	name.Create("Cerdo_Volador");
}

Cerdo_Volador::~Cerdo_Volador() {}

bool Cerdo_Volador::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	speed = parameters.attribute("speed").as_float();

	return true;
}

bool Cerdo_Volador::Start() {


	//run
	idleRight.LoadAnimations("Idleright", "cerdoVolador");
	idleRight.speed = 0.16f;

	idleLeft.LoadAnimations("Idleleft", "cerdoVolador");
	idleLeft.speed = 0.16f;

	dead.LoadAnimations("Dead", "cerdoVolador");
	dead.speed = 0.167f;

	currentAnimation = &idleRight;

	//initilize textures
	texture = app->tex->Load(texturePath);
	pathTexture = app->tex->Load("Assets/Textures/tomate.png");
	pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::KINEMATIC); // porque no funciona ?? 
	pbody->ctype = ColliderType::CERDO;
	pbody->listener = this;
	posA = position.x - 50;
	posB = position.x + 50;

	//initialTransform = pbody->body->GetTransform();

	//LoadAnimations();

	return true;
}

bool Cerdo_Volador::Update(float dt)
{

	if (looksRight)
	{
		currentAnimation = &idleRight;
	}
	else
	{
		currentAnimation = &idleRight;
	}

	playerTilePos = app->map->WorldToMap(app->scene->player->position.x + 16, app->scene->player->position.y);
	enemyPosition = app->map->WorldToMap(position.x + 8, position.y);

	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	distance = sqrt(pow(playerTilePos.x - enemyPosition.x, 2) + pow(playerTilePos.y - enemyPosition.y, 2));

	if (distance < 4)
	{
		//currentAnimation = &attack;
		currentVelocity.x = 0;
		currentVelocity.y = 0;
		pbody->body->SetLinearVelocity(currentVelocity);
	}
	else if (distance >= 4 && distance <= 10)
	{
		app->map->pathfinding->CreatePath(enemyPosition, playerTilePos);
		/*lastPath = app->map->pathfinding->GetLastPath();*/

		//if (lastPath->Count() > 0)
		//{
		//	const iPoint* nextTile;
		//	nextTile = lastPath->At(lastPath->Count() - 1);

		//	if (nextTile->x == position.x)
		//	{

		//	}
		//	else if (nextTile->x < position.x)
		//	{
		//		currentAnimation = &runRight;
		//		currentVelocity.x = speed * 2.5;
		//		pbody->body->SetLinearVelocity(currentVelocity);
		//	}
		//	else
		//	{
		//		currentVelocity.x = -speed * 2.5;
		//		currentAnimation = &runLeft;
		//		pbody->body->SetLinearVelocity(currentVelocity);
		//	}
		//}

		if (app->map->pathfinding->IsWalkable(playerTilePos) != 0)
		{
			isFollowingPlayer = true;
			if (position.x < app->scene->player->position.x)
			{
				looksRight = true;
				currentVelocity.x = speed * 2.5;
				pbody->body->SetLinearVelocity(currentVelocity);
				if (position.y + 10 < app->scene->player->position.y)
				{
					currentVelocity.y = speed * 2.5;
					pbody->body->SetLinearVelocity(currentVelocity);
				}
				else if (position.y - 10 > app->scene->player->position.y)
				{
					currentVelocity.y = -speed * 2.5;
					pbody->body->SetLinearVelocity(currentVelocity);
				}
				else
				{
					currentVelocity.y = 0;
					pbody->body->SetLinearVelocity(currentVelocity);
				}
			}
			else if (position.x > app->scene->player->position.x)
			{
				looksRight = false;
				currentVelocity.x = -speed * 2.5;
				pbody->body->SetLinearVelocity(currentVelocity);
				if (position.y + 10 < app->scene->player->position.y)
				{
					currentVelocity.y = speed * 2.5;
					pbody->body->SetLinearVelocity(currentVelocity);
				}
				else if (position.y - 10 > app->scene->player->position.y)
				{
					currentVelocity.y = -speed * 2.5;
					pbody->body->SetLinearVelocity(currentVelocity);
				}
				else
				{
					currentVelocity.y = 0;
					pbody->body->SetLinearVelocity(currentVelocity);
				}
			}
		}
	}
	else
	{
		currentVelocity.x = 0;
		currentVelocity.y = 0;
		pbody->body->SetLinearVelocity(currentVelocity);
		app->map->pathfinding->ClearLastPath();
	}

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 18;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15;

	if (looksRight)
	{
		app->render->DrawTexture(texture, position.x, position.y - 4, &currentAnimation->GetCurrentFrame());
	}
	else
	{
		app->render->DrawTexture(texture, position.x, position.y - 4, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	}

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

bool Cerdo_Volador::CleanUp()
{
	return true;
}

void Cerdo_Volador::OnDeath()
{
	currentAnimation = &dead;
	currentAnimation->loopCount = 0;
	app->entityManager->DestroyEntity(this);
	app->physics->world->DestroyBody(pbody->body);
}

void Cerdo_Volador::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
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