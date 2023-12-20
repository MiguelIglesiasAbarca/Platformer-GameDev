#include "Rey.h"
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

Rey::Rey() : Entity(EntityType::REY)
{
	name.Create("Rey");
}

Rey::~Rey() {}

bool Rey::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	king_FXid = app->audio->LoadFx("Assets/Audio/Fx/pig_explosion_FX.wav");

	return true;
}

bool Rey::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangle(position.x + 50, position.y + 60,100,120, bodyType::STATIC);
	pbody->ctype = ColliderType::REY;
	pbody->listener = this;

	return true;
}

bool Rey::Update(float dt)
{
	app->render->DrawTexture(texture, position.x, position.y);
	return true;
}

bool Rey::CleanUp()
{
	return true;
}

void Rey::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		app->audio->PlayFx(king_FXid, 0);
		texturePath = "Assets/Textures/Throneyum.png";
		texture = app->tex->Load(texturePath);
		break;
	}

}