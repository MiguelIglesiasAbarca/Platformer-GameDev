#include "Enemy_A.h"
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

Enemy_A::Enemy_A() : Entity(EntityType::ENEMY_A)
{
	name.Create("Enemy_A");
}

Enemy_A::~Enemy_A() {}


bool Enemy_A::Awake() {

	//position.x = parameters.attribute("x").as_int();
	position.x = 100;
	//position.y = parameters.attribute("y").as_int();
	position.y = 100;

	texturePath = parameters.attribute("texturepath").as_string();
	//texturePath = parameters.attribute("texturepath").as_string();
	//speed = parameters.attribute("speed").as_float();

	return true;
}

bool Enemy_A::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 100, position.y, 12, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY_A;

	return true;
}

bool Enemy_A::Update(float dt)
{
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 18;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15;

	app->render->DrawTexture(texture, position.x - 5, position.y - 2);
	//currentAnimation->Update();

	return true;
}

bool Enemy_A::CleanUp()
{
	return true;
}

void Enemy_A::OnCollision(PhysBody* physA, PhysBody* physB) {

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