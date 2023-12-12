#include "Cerdo_patrullador.h"
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

	//initilize textures
	/*pathTexture = app->tex->Load("Assets/Textures/tomate.png");*/
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x, position.y, 10, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::CERDO;
	pbody->listener = this;
	posA = position.x - 50;
	posB = position.x + 50;

	//enemyPbody = app->physics->CreateRectangleSensor(position.x, position.y, 30, 54, bodyType::KINEMATIC);
	//enemyPbody->ctype = ColliderType::ENEMY;

	//initialTransform = pbody->body->GetTransform();

	//LoadAnimations();

	return true;
}

bool CerdoPatrullador::Update(float dt)
{

	//playerTilePos = app->map->WorldToMap(app->scene->player->position.x + 16, app->scene->player->position.y);
	//cerdoPosition = app->map->WorldToMap(position.x + 8, position.y);

	

	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	currentVelocity.y += 0.5;

	/*app->map->pathfinding->CreatePath(cerdoPosition, playerTilePos);*/

	if (position.x >= posB)
	{
		direction = false;
	}
	if (position.x <= posA)
	{
		direction = true;
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

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 18;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15;

	app->render->DrawTexture(texture, position.x, position.y);

	/*if (app->physics->debug)
	{
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(pathTexture, pos.x + 8, pos.y + 8);
		}
	}*/

	return true;
}

bool CerdoPatrullador::CleanUp()
{
	return true;
}


void CerdoPatrullador::OnCollision(PhysBody* physA, PhysBody* physB) {

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
