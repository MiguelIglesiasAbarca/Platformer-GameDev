#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	configNode = config;
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	for (pugi::xml_node itemNode = config.child("trap"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::TRAP);
		item->parameters = itemNode;
	}

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	for (pugi::xml_node enemiesNode = config.child("enemies"); enemiesNode; enemiesNode = enemiesNode.next_sibling("enemies"))
	{
		for (pugi::xml_node cerdoNode = enemiesNode.child("cerdo"); cerdoNode; cerdoNode = cerdoNode.next_sibling("cerdo"))
		{
			Cerdo* cerdo = (Cerdo*)app->entityManager->CreateEntity(EntityType::CERDO);
			cerdo->parameters = cerdoNode;
		}

		for (pugi::xml_node cerdoPatrulladorNode = enemiesNode.child("cerdoPatrullador"); cerdoPatrulladorNode; cerdoPatrulladorNode = cerdoPatrulladorNode.next_sibling("cerdoPatrullador"))
		{
			CerdoPatrullador* cerdoPatrullador = (CerdoPatrullador*)app->entityManager->CreateEntity(EntityType::CERDO_PATRULLADOR);
			cerdoPatrullador->parameters = cerdoPatrulladorNode;
		}

		for (pugi::xml_node dragonNode = enemiesNode.child("dragon"); dragonNode; dragonNode = dragonNode.next_sibling("dragon"))
		{
			Dragon* dragon = (Dragon*)app->entityManager->CreateEntity(EntityType::DRAGON);
			dragon->parameters = dragonNode;
		}

		for (pugi::xml_node cerdoVoladorNode = enemiesNode.child("cerdoVolador"); cerdoVoladorNode; cerdoVoladorNode = cerdoVoladorNode.next_sibling("cerdoVolador"))
		{
			Cerdo_Volador* cerdoVolador = (Cerdo_Volador*)app->entityManager->CreateEntity(EntityType::CERDO_VOLADOR);
			cerdoVolador->parameters = cerdoVoladorNode;
		}
	}

	for (pugi::xml_node foodNode = config.child("comida"); foodNode; foodNode = foodNode.next_sibling("comida"))
	{
		for (pugi::xml_node tomateNode = foodNode.child("tomate"); tomateNode; tomateNode = tomateNode.next_sibling("tomate"))
		{
			Food* tomate = (Food*)app->entityManager->CreateEntity(EntityType::COMIDA);
			tomate->parameters = tomateNode;
		}

		for (pugi::xml_node quesoNode = foodNode.child("queso"); quesoNode; quesoNode = quesoNode.next_sibling("queso"))
		{
			Food* queso = (Food*)app->entityManager->CreateEntity(EntityType::COMIDA);
			queso->parameters = quesoNode;
		}

		for (pugi::xml_node masaNode = foodNode.child("masa"); masaNode; masaNode = masaNode.next_sibling("masa"))
		{
			Food* masa = (Food*)app->entityManager->CreateEntity(EntityType::COMIDA);
			masa->parameters = masaNode;
		}
	}

	for (pugi::xml_node reyNode = config.child("rey"); reyNode; reyNode = reyNode.next_sibling("rey"))
	{
		Rey* rey = (Rey*)app->entityManager->CreateEntity(EntityType::REY);
		rey->parameters = reyNode;
	}

	if (config.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->name = config.child("map").attribute("name").as_string();
		app->map->path = config.child("map").attribute("path").as_string();
	}

	return ret;
}

// Called before Fthe first frame
bool Scene::Start()
{
	fondo0 = app->tex->Load(configNode.child("background").attribute("path").as_string());
	fondo1 = app->tex->Load(configNode.child("background1").attribute("path").as_string());
	fondo2 = app->tex->Load(configNode.child("background2").attribute("path").as_string());
	fondo3 = app->tex->Load(configNode.child("background3").attribute("path").as_string());
	fondo4 = app->tex->Load(configNode.child("background4").attribute("path").as_string());

	app->win->GetWindowSize(windowW, windowH);

	app->audio->PlayMusic("Assets/Audio/Music/background_music.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{	
	if (player->position.x <= 600)
	{
		app->render->camera.y = -player->position.y + 500;
		app->render->camera.x = -100;

		if (player->position.y >= 3450)
		{
			app->render->camera.y = -2950;
		}
	}
	else if (player->position.x > 600 && player->position.x <= 2850)
	{
		app->render->camera.y = -player->position.y + 500;
		app->render->camera.x = -player->position.x + 500;

		if (player->position.y >= 3450)
		{
			app->render->camera.y = -2950;
		}
	}
	else if (player->position.x > 2850)
	{
		app->render->camera.y = -player->position.y + 500;
		app->render->camera.x = -2350;

		if (player->position.y >= 3450)
		{
			app->render->camera.y = -2950;
		}
	}

	SDL_Rect Rectfondo0{ 0,0,576*scale,324*scale};
	SDL_Rect Rectfondo1{ 0,0,576*scale,324*scale};
	SDL_Rect Rectfondo2{ 0,0,576*scale,324*scale};
	SDL_Rect Rectfondo3{ 0,0,576*scale,324*scale};
	SDL_Rect Rectfondo4{ 0,0,576*scale,324*scale};
	app->render->DrawTexture(fondo0, -100, 0, &Rectfondo0);
	app->render->DrawTexture(fondo1, -150, -100, &Rectfondo0, SDL_FLIP_NONE, 0.4f);
	app->render->DrawTexture(fondo2, -150, 110, &Rectfondo0, SDL_FLIP_NONE, 0.6f);
	app->render->DrawTexture(fondo3, -150, 110, &Rectfondo0, SDL_FLIP_NONE, 0.7f);
	app->render->DrawTexture(fondo4, -150, 0, &Rectfondo0, SDL_FLIP_NONE,0.4f);

	// L14: TODO 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool Scene::LoadState(pugi::xml_node node) 
{
	player->position.x = node.child("player").attribute("x").as_int();
	player->position.y = node.child("player").attribute("y").as_int(); 
	player->godMode = node.child("pconditions").attribute("godMode").as_bool();
	player->isDead = node.child("pconditions").attribute("isAlive").as_bool();
	player->isJumping = node.child("pconditions").attribute("isJumping").as_bool();
	player->running = node.child("pconditions").attribute("running").as_bool();
	player->looksRight = node.child("pconditions").attribute("left_right").as_bool();

	player->pbody->body->SetTransform({ PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player->position.y) }, 0);

	return true;
}

bool Scene::SaveState(pugi::xml_node node) 
{
	pugi::xml_node posNode = node.append_child("player");
	posNode.append_attribute("x").set_value(player->position.x);
	posNode.append_attribute("y").set_value(player->position.y);

	pugi::xml_node pconditionsNode = node.append_child("pconditions");
	pconditionsNode.append_attribute("godMode").set_value(player->godMode);
	pconditionsNode.append_attribute("isDead").set_value(player->isDead);
	pconditionsNode.append_attribute("isJumping").set_value(player->isJumping);
	pconditionsNode.append_attribute("running").set_value(player->running);
	pconditionsNode.append_attribute("left_right").set_value(player->looksRight);

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
