#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
//#include "SceneLogo.h"
#include "SceneIntro.h"
#include "Scenepause.h"
//#include "LastScreen.h"
#include "Scene.h"
//#include "SceneSettings.h"
#include "Map.h"
//#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"

ScenePause::ScenePause(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("sceneLogo");
}

// Destructor
ScenePause::~ScenePause()
{}

// Called before render is available
bool ScenePause::Awake()
{
	LOG("Loading Logo Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool ScenePause::Start()
{


	//Pause
	Pause_1 = app->tex->Load("Assets/Textures/pausesnormal.png");
	Pause_2 = app->tex->Load("Assets/Textures/pauseresumehover.png");
	Pause_3 = app->tex->Load("Assets/Textures/pauseresumepress.png");
	Pause_4 = app->tex->Load("Assets/Textures/pausesettingshover.png");
	Pause_5 = app->tex->Load("Assets/Textures/pausesettingspress.png");
	Pause_6 = app->tex->Load("Assets/Textures/pausequithover.png");
	Pause_7 = app->tex->Load("Assets/Textures/pausequitpress.png");
	

	app->guiManager->Enable();
	app->scene->Disable();

	btn1 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Resume", { 59, 103, 906, 257 }, this);
	btn2 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Settings", { 285, 421, 453, 108 }, this);
	btn3 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Quit", { 287, 584, 450, 108 }, this);
	


	//Habilita los botones
	btn1->state = GuiControlState::NORMAL;
	btn2->state = GuiControlState::NORMAL;
	btn3->state = GuiControlState::NORMAL;


	return true;
}

// Called each loop iteration
bool ScenePause::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {

		
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
			app->scenepause->Disable();
			app->guiManager->Enable();
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
			app->scenepause->Disable();
			app->guiManager->Disable();
		}

		if (btn3->state == GuiControlState::FOCUSED)
		{
			currentTexture = Pause_6;
		}
		else if (btn3->state == GuiControlState::PRESSED)
		{
			currentTexture = Pause_7;
			// Acción cuando se presiona el botón "Quit"
			app->scenepause->Disable();
			app->guiManager->Disable();
		}
	}

	

	return true;
}

// Called each loop iteration
bool ScenePause::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool ScenePause::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(currentTexture, 0, 0, NULL, SDL_FLIP_NONE, 0);

	return ret;
}

bool ScenePause::OnGuiMouseClickEvent(GuiControl* control) {

	return true;
}

// Called before quitting
bool ScenePause::CleanUp()
{
	LOG("Freeing best logo ever scene");

	/*app->tex->UnLoad(Pause_1);
	app->tex->UnLoad(Pause_2);
	app->tex->UnLoad(Pause_3);
	app->tex->UnLoad(Pause_4);
	app->tex->UnLoad(Pause_5);
	app->tex->UnLoad(Pause_6);
	app->tex->UnLoad(Pause_7);
	app->tex->UnLoad(Pause_8);
	app->tex->UnLoad(Pause_9);

	app->guiManager->DestroyGuiControl(btn1);
	app->guiManager->DestroyGuiControl(btn2);
	app->guiManager->DestroyGuiControl(btn3);
	app->guiManager->DestroyGuiControl(btn4);*/

	return true;
}