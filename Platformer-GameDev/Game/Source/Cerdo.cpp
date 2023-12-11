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

	//initilize textures
	//pathTexture = app->tex->Load("Assets/Textures/path.png");
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x, position.y, 10, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::CERDO;
	pbody->listener = this;

	//enemyPbody = app->physics->CreateRectangleSensor(position.x, position.y, 30, 54, bodyType::KINEMATIC);
	//enemyPbody->ctype = ColliderType::ENEMY;
	//enemyPbody->listener = this;

	//initialTransform = pbody->body->GetTransform();

	//LoadAnimations();

	return true;
}

bool Cerdo::Update(float dt)
{

	playerTilePos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();
	
	currentVelocity.y += 0.5;

	app->map->pathfinding->CreatePath(position, playerTilePos);

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 18;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15;

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Cerdo::CleanUp()
{
	return true;
}


void Cerdo::OnCollision(PhysBody* physA, PhysBody* physB) {

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
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}
