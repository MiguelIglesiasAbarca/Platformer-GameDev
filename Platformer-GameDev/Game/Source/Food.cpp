#include "Food.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Food::Food() : Entity(EntityType::COMIDA)
{
	name.Create("Food");
}

Food::~Food() {}

bool Food::Awake() {

	/*position.x = 200;
	position.y = 1700;
	texture = app->tex->Load("Assets/Textures/tomate.png");*/

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Food::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x, position.y, 16, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ITEM;

	return true;
}

bool Food::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics. 

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x+8, position.y+8);

	return true;
}
bool Food::CleanUp()
{
	return true;
}


