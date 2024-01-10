#include "Intro.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"


Intro::Intro() : Module()
{
	name.Create("intro");
}

// Destructor
Intro::~Intro()
{

}

bool Intro::Awake(pugi::xml_node& config)
{
	configNode = config;
	LOG("Loading Intro");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	/*for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}*/

	/*for (pugi::xml_node itemNode = config.child("trap"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::TRAP);
		item->parameters = itemNode;
	}*/

	/*if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}*/

	/*for (pugi::xml_node enemiesNode = config.child("enemies"); enemiesNode; enemiesNode = enemiesNode.next_sibling("enemies"))
	{
		for (pugi::xml_node cerdoNode = enemiesNode.child("cerdo"); cerdoNode; cerdoNode = cerdoNode.next_sibling("cerdo"))
		{
			Cerdo* cerdo = (Cerdo*)app->entityManager->CreateEntity(EntityType::CERDO);
			cerdo->parameters = cerdoNode;
		}
		Cerdolista;

		for (pugi::xml_node cerdoPatrulladorNode = enemiesNode.child("cerdoPatrullador"); cerdoPatrulladorNode; cerdoPatrulladorNode = cerdoPatrulladorNode.next_sibling("cerdoPatrullador"))
		{
			CerdoPatrullador* cerdoPatrullador = (CerdoPatrullador*)app->entityManager->CreateEntity(EntityType::CERDO_PATRULLADOR);
			cerdoPatrullador->parameters = cerdoPatrulladorNode;
		}
		CerdoPatrulladorlista;

		for (pugi::xml_node dragonNode = enemiesNode.child("dragon"); dragonNode; dragonNode = dragonNode.next_sibling("dragon"))
		{
			Dragon* dragon = (Dragon*)app->entityManager->CreateEntity(EntityType::DRAGON);
			dragon->parameters = dragonNode;
		}
		Dragonlista;

		for (pugi::xml_node cerdoVoladorNode = enemiesNode.child("cerdoVolador"); cerdoVoladorNode; cerdoVoladorNode = cerdoVoladorNode.next_sibling("cerdoVolador"))
		{
			Cerdo_Volador* cerdoVolador = (Cerdo_Volador*)app->entityManager->CreateEntity(EntityType::CERDO_VOLADOR);
			cerdoVolador->parameters = cerdoVoladorNode;
		}
		CerdoVoladorlista;
	}*/

	/*for (pugi::xml_node foodNode = config.child("comida"); foodNode; foodNode = foodNode.next_sibling("comida"))
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
	}*/

	/*for (pugi::xml_node reyNode = config.child("rey"); reyNode; reyNode = reyNode.next_sibling("rey"))
	{
		Rey* rey = (Rey*)app->entityManager->CreateEntity(EntityType::REY);
		rey->parameters = reyNode;
	}*/

	/*app->entityManager->GetCerdo(Cerdolista);
	app->entityManager->GetCerdoVolador(CerdoVoladorlista);
	app->entityManager->GetCerdoPatrullador(CerdoPatrulladorlista);
	app->entityManager->GetDragon(Dragonlista);*/

	//if (config.child("map")) {
	//	//Get the map name from the config file and assigns the value in the module
	//	app->map->name = config.child("map").attribute("name").as_string();
	//	app->map->path = config.child("map").attribute("path").as_string();
	//}

	return ret;
}

bool Intro::Start()
{

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

bool Intro::PreUpdate()
{
	return true;
}

bool Intro::Update(float dt)
{
	return true;
}

// Called each loop iteration
bool Intro::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool Intro::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
