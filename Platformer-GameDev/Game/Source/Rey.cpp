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
	//speed = parameters.attribute("speed").as_float();
	king_FXid = app->audio->LoadFx("Assets/Audio/Fx/pig_explosion_FX.wav");

	return true;
}

bool Rey::Start() {


	////run
	//runRight.LoadAnimations("Bombright", "cerdoBomba");
	//runRight.speed = 0.16f;

	//runLeft.LoadAnimations("Bombleft", "cerdoBomba");
	//runLeft.speed = 0.16f;
	////dead
	//dead.LoadAnimations("Dead", "cerdoBomba");
	//dead.speed = 0.25f;
	//watifok
	/*watifokIn.LoadAnimations("Watifokin", "cerdoBomba");
	watifokIn.speed = 0.05f;*/

	/*currentAnimation = &runRight;*/

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangle(position.x + 50, position.y + 60,100,120, bodyType::STATIC);
	pbody->ctype = ColliderType::REY;
	pbody->listener = this;

	//initialTransform = pbody->body->GetTransform();

	//LoadAnimations();

	return true;
}

bool Rey::Update(float dt)
{

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
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::DAMAGE:
		LOG("Collision DAMAGE");
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