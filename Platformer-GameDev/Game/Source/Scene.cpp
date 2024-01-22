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
#include "GuiControl.h"
#include "GuiManager.h"

Scene::Scene(App* app, bool start_enabled) : Module(app, start_enabled)
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

		for (pugi::xml_node BossNode = enemiesNode.child("boss"); BossNode; BossNode = BossNode.next_sibling("boss"))
		{
			Boss* boss = (Boss*)app->entityManager->CreateEntity(EntityType::BOSS);
			boss->parameters = BossNode;
		}
		BossLista;
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

	for (pugi::xml_node liveNode = config.child("corazon"); liveNode; liveNode = liveNode.next_sibling("corazon"))
	{
		Lives* corazon = (Lives*)app->entityManager->CreateEntity(EntityType::VIDA);
		corazon->parameters = liveNode;
	}

	for (pugi::xml_node reyNode = config.child("rey"); reyNode; reyNode = reyNode.next_sibling("rey"))
	{
		Rey* rey = (Rey*)app->entityManager->CreateEntity(EntityType::REY);
		rey->parameters = reyNode;
	}

	app->entityManager->GetCerdo(Cerdolista);
	app->entityManager->GetCerdoVolador(CerdoVoladorlista);
	app->entityManager->GetCerdoPatrullador(CerdoPatrulladorlista);
	app->entityManager->GetDragon(Dragonlista);

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

	hearts_tex = app->tex->Load(configNode.child("hearts_tex").attribute("path").as_string());

	app->win->GetWindowSize(windowW, windowH);

	//app->audio->PlayMusic("Assets/Audio/Music/background_music.ogg");

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

	SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10,120 ,20 };
	gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON,hearts_tex, 1, "", btPos, this);

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
	if (player->position.x <= -100)
	{
		app->render->camera.y = -player->position.y + 500;
		app->render->camera.x = -100;

		if (player->position.y >= 3450)
		{
			app->render->camera.y = -2950;
		}
	}
	else if (player->position.x > -100 && player->position.x <= 2850)
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

	for (int Cerdocount = 0; Cerdocount < Cerdolista.Count(); Cerdocount++) {

		Entity* cerdo = Cerdolista.At(Cerdocount)->data;

		// Convert the current count to a string for constructing XML attribute names.
		std::string count = std::to_string(Cerdocount + 1);

		// Update the position of the slime entity based on XML attributes.
		cerdo->position.x = node.child(("Cerdo" + count).c_str()).child("CerdoPosition").attribute("x").as_int();
		cerdo->position.y = node.child(("Cerdo" + count).c_str()).child("CerdoPosition").attribute("y").as_int();
		cerdo->isDead = node.child(("Cerdo" + count).c_str()).child("CerdoPosition").attribute("isDead").as_bool();
		cerdo->tp = true;
	}

	for (int CerdoPatcount = 0; CerdoPatcount < CerdoPatrulladorlista.Count(); CerdoPatcount++) {

		Entity* cerdopat = CerdoPatrulladorlista.At(CerdoPatcount)->data;

		// Convert the current count to a string for constructing XML attribute names.
		std::string count = std::to_string(CerdoPatcount + 1);

		// Update the position of the slime entity based on XML attributes.
		cerdopat->position.x = node.child(("Cerdopat" + count).c_str()).child("CerdoPatPosition").attribute("x").as_int();
		cerdopat->position.y = node.child(("Cerdopat" + count).c_str()).child("CerdoPatPosition").attribute("y").as_int();
		cerdopat->isDead = node.child(("Cerdopat" + count).c_str()).child("CerdoPatPosition").attribute("isDead").as_bool();
		cerdopat->tp = true;
	}

	for (int CerdoVolcount = 0; CerdoVolcount < CerdoVoladorlista.Count(); CerdoVolcount++) {

		Entity* cerdovol = CerdoVoladorlista.At(CerdoVolcount)->data;

		// Convert the current count to a string for constructing XML attribute names.
		std::string count = std::to_string(CerdoVolcount + 1);

		// Update the position of the slime entity based on XML attributes.
		cerdovol->position.x = node.child(("Cerdovol" + count).c_str()).child("CerdoVolPosition").attribute("x").as_int();
		cerdovol->position.y = node.child(("Cerdovol" + count).c_str()).child("CerdoVolPosition").attribute("y").as_int();
		cerdovol->isDead = node.child(("Cerdovol" + count).c_str()).child("CerdoVolPosition").attribute("isDead").as_bool();
		cerdovol->tp = true;
	}

	for (int Dragoncount = 0; Dragoncount < Dragonlista.Count(); Dragoncount++) {

		Entity* dragon = Dragonlista.At(Dragoncount)->data;

		// Convert the current count to a string for constructing XML attribute names.
		std::string count = std::to_string(Dragoncount + 1);

		// Update the position of the slime entity based on XML attributes.
		dragon->position.x = node.child(("Dragon" + count).c_str()).child("DragonPosition").attribute("x").as_int();
		dragon->position.y = node.child(("Dragon" + count).c_str()).child("DragonPosition").attribute("y").as_int();
		dragon->isDead = node.child(("Dragon" + count).c_str()).child("DragonPosition").attribute("isDead").as_bool();
		dragon->tp = true;
	}
	
	List<Entity> Cerdolista;
	List<Entity> CerdoPatrulladorlista;
	List<Entity> CerdoVoladorlista;
	List<Entity> Dragonlista;

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

	for (int CerdoCount = 0; CerdoCount < Cerdolista.Count(); CerdoCount++) {
		std::string count = std::to_string(CerdoCount + 1);
		Entity* cerdo = Cerdolista.At(CerdoCount)->data;
		pugi::xml_node enemyNode = node.append_child(("Cerdo" + count).c_str()).append_child("CerdoPosition");
		enemyNode.append_attribute("x").set_value(cerdo->position.x);
		enemyNode.append_attribute("y").set_value(cerdo->position.y);
		enemyNode.append_attribute("isDead").set_value(cerdo->isDead);
	}
	for (int CerdoPatCount = 0; CerdoPatCount < CerdoPatrulladorlista.Count(); CerdoPatCount++) {
		std::string count = std::to_string(CerdoPatCount + 1);
		Entity* cerdopat = CerdoPatrulladorlista.At(CerdoPatCount)->data;
		pugi::xml_node enemyNode = node.append_child(("Cerdopat" + count).c_str()).append_child("CerdoPatPosition");
		enemyNode.append_attribute("x").set_value(cerdopat->position.x);
		enemyNode.append_attribute("y").set_value(cerdopat->position.y);
		enemyNode.append_attribute("isDead").set_value(cerdopat->isDead);
	}
	for (int CerdoVolCount = 0; CerdoVolCount < CerdoVoladorlista.Count(); CerdoVolCount++) {
		std::string count = std::to_string(CerdoVolCount + 1);
		Entity* cerdovol = CerdoVoladorlista.At(CerdoVolCount)->data;
		pugi::xml_node enemyNode = node.append_child(("Cerdovol" + count).c_str()).append_child("CerdoVolPosition");
		enemyNode.append_attribute("x").set_value(cerdovol->position.x);
		enemyNode.append_attribute("y").set_value(cerdovol->position.y);
		enemyNode.append_attribute("isDead").set_value(cerdovol->isDead);
	}
	for (int DragonCount = 0; DragonCount < Dragonlista.Count(); DragonCount++) {
		std::string count = std::to_string(DragonCount + 1);
		Entity* dragon = Dragonlista.At(DragonCount)->data;
		pugi::xml_node enemyNode = node.append_child(("Dragon" + count).c_str()).append_child("DragonPosition");
		enemyNode.append_attribute("x").set_value(dragon->position.x);
		enemyNode.append_attribute("y").set_value(dragon->position.y);
		enemyNode.append_attribute("isDead").set_value(dragon->isDead);
	}

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: TODO 5: Implement the OnGuiMouseClickEvent method
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}
