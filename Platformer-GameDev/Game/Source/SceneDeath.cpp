#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
//#include "SceneLogo.h"
#include "SceneIntro.h"
#include "SceneDeath.h"
//#include "LastScreen.h"
#include "Scene.h"
//#include "SceneSettings.h"
#include "Map.h"
//#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"

SceneDeath::SceneDeath(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("sceneDeath");
}

// Destructor
SceneDeath::~SceneDeath()
{}

// Called before render is available
bool SceneDeath::Awake()
{
	LOG("Loading Death Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneDeath::Start()
{


	//Pause
	Pause_1 = app->tex->Load("Assets/Textures/muertenormal.png");
	Pause_2 = app->tex->Load("Assets/Textures/muertecheckhover.png");
	Pause_3 = app->tex->Load("Assets/Textures/muertecheckpress.png");
	Pause_4 = app->tex->Load("Assets/Textures/muerteiniciohover.png");
	Pause_5 = app->tex->Load("Assets/Textures/muerteiniciopress.png");



	app->guiManager->Enable();
	app->scene->Disable();
	app->map->Disable();
	app->entityManager->Disable();

	btn1 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Resume", { 545, 322, 434, 150 }, this);
	btn2 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Settings", { 549, 538, 434, 150 }, this);




	//Habilita los botones
	btn1->state = GuiControlState::NORMAL;
	btn2->state = GuiControlState::NORMAL;



	return true;
}

// Called each loop iteration
bool SceneDeath::PreUpdate()
{
	
	if (app->scene->player->vida == 0) {


		currentTexture = Pause_1;

		if (btn1->state == GuiControlState::FOCUSED)
		{
			currentTexture = Pause_2;
		}
		else if (btn1->state == GuiControlState::PRESSED)
		{
			currentTexture = Pause_3;
			// Acción cuando se presiona el botón "Resume"
			app->sceneintro->Disable();
			//app->scenepause->Disable();
			app->guiManager->Disable();
			app->scene->Enable();
		}

		if (btn2->state == GuiControlState::FOCUSED)
		{
			currentTexture = Pause_4;
		}
		else if (btn2->state == GuiControlState::PRESSED)
		{
			currentTexture = Pause_5;
			// Acción cuando se presiona el botón "Settings"
			app->scene->Enable();
			//app->sceneSettings->Enable()
			//app->scenepause->Disable();
			app->guiManager->Disable();
		}
	}



	return true;
}

// Called each loop iteration
bool SceneDeath::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool SceneDeath::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(currentTexture, 0, 0, NULL, SDL_FLIP_NONE, 0);

	return ret;
}

bool SceneDeath::OnGuiMouseClickEvent(GuiControl* control) {

	return true;
}

// Called before quitting
bool SceneDeath::CleanUp()
{
	LOG("Freeing best logo ever scene");

	// Unload textures
	app->tex->UnLoad(Pause_1);
	app->tex->UnLoad(Pause_2);
	app->tex->UnLoad(Pause_3);
	app->tex->UnLoad(Pause_4);
	app->tex->UnLoad(Pause_5);

	// Destroy GUI controls
	/*app->guiManager->DestroyGuiControl(btn1);
	app->guiManager->DestroyGuiControl(btn2);*/

	return true;
}