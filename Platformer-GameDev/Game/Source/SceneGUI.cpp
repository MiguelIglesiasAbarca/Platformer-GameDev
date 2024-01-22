#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneGUI.h"
#include "Scene.h"
#include "Map.h"
#include "EntityManager.h"


#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"

#include "Timer.h"

SceneGUI::SceneGUI(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("sceneGUI");
}

// Destructor
SceneGUI::~SceneGUI()
{}

// Called before render is available
bool SceneGUI::Awake(pugi::xml_node& config)
{
	configNode = config;
	LOG("Loading SceneGUI");
	bool ret = true;

	timer = Timer();
	timer.Start();
	timerPaused = false;
	return ret;
}

// Called before Fthe first frame
bool SceneGUI::Start()
{
	LOG("Loading SceneIntro Assets");
	bool ret = true;

	
	app->win->GetWindowSize(windowW, windowH);
	corazon1 = app->tex->Load("Assets/Textures/corazon1.png");
	corazon2 = app->tex->Load("Assets/Textures/corazon2.png");
	corazon3 = app->tex->Load("Assets/Textures/gatoguapo.png");

	//app->audio->PlayMusic("Assets/Audio/Music/background_music.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	/*menu = app->tex->Load("Assets/Textures/inicionormal.png");
	playHover = app->tex->Load("Assets/Textures/inicioplayhover.png");
	continueHover = app->tex->Load("Assets/Textures/iniciocontinuehover.png");
	settingsHover = app->tex->Load("Assets/Textures/iniciosettingshover.png");
	creditsHover = app->tex->Load("Assets/Textures/iniciocreditshover.png");
	exitHover = app->tex->Load("Assets/Textures/inicioquithover.png");
	playClick = app->tex->Load("Assets/Textures/inicioplayclick.png");
	continueClick = app->tex->Load("Assets/Textures/iniciocontinueclick.png");
	settingsClick = app->tex->Load("Assets/Textures/iniciosettingsclick.png");
	creditsClick = app->tex->Load("Assets/Textures/iniciocreditsclick.png");
	exitClick = app->tex->Load("Assets/Textures/inicioquitclick.png");*/


	/*playButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Play", { 125, 311, 371, 124 }, this);
	continueButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Continue", { 543, 307, 381, 134 }, this);
	settingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", { 354, 460, 316, 76 }, this);
	creditsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Credits", { 354, 562, 316, 76 }, this);
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Exit", { 416, 676, 200, 48 }, this);

	playButton->state = GuiControlState::NORMAL;
	continueButton->state = GuiControlState::NORMAL;
	settingsButton->state = GuiControlState::NORMAL;
	creditsButton->state = GuiControlState::NORMAL;
	exitButton->state = GuiControlState::NORMAL;*/
	return true;
}

// Called each loop iteration
bool SceneGUI::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneGUI::Update(float dt)
{
	//dibujamos
	/*SDL_Rect RectFondoInicial{ 0, 0, windowW - 0, windowH - 0 };
	if (!timerPaused) app->render->DrawTexture(gatitorico, 0, 0, NULL, SDL_FLIP_NONE, 0);
	else
	{
		if (playButton->state == GuiControlState::FOCUSED)
		{
			app->render->DrawTexture(playHover, 0, 0, NULL, SDL_FLIP_NONE, 0);
		}
		else if (playButton->state == GuiControlState::PRESSED)
		{
			app->scene->Enable();
			app->entityManager->Enable();
			app->map->Enable();
			this->Disable();
			app->guiManager->Disable();
		}
		else if (continueButton->state == GuiControlState::FOCUSED)
		{
			app->render->DrawTexture(continueHover, 0, 0, NULL, SDL_FLIP_NONE, 0);
		}
		else if (continueButton->state == GuiControlState::PRESSED)
		{
			app->render->DrawTexture(continueClick, 0, 0, NULL, SDL_FLIP_NONE, 0);
		}
		else if (settingsButton->state == GuiControlState::FOCUSED)
		{
			app->render->DrawTexture(settingsHover, 0, 0, NULL, SDL_FLIP_NONE, 0);
		}
		else if (settingsButton->state == GuiControlState::PRESSED)
		{
			app->render->DrawTexture(settingsClick, 0, 0, NULL, SDL_FLIP_NONE, 0);
		}
		else if (creditsButton->state == GuiControlState::FOCUSED)
		{
			app->render->DrawTexture(creditsHover, 0, 0, NULL, SDL_FLIP_NONE, 0);
		}
		else if (creditsButton->state == GuiControlState::PRESSED)
		{
			app->render->DrawTexture(creditsClick, 0, 0, NULL, SDL_FLIP_NONE, 0);
		}
		else if (exitButton->state == GuiControlState::FOCUSED)
		{
			app->render->DrawTexture(exitHover, 0, 0, NULL, SDL_FLIP_NONE, 0);
		}
		else if (exitButton->state == GuiControlState::PRESSED)
		{
			app->render->DrawTexture(exitClick, 0, 0, NULL, SDL_FLIP_NONE, 0);
		}
		else
		{
			app->render->DrawTexture(menu, 0, 0, NULL, SDL_FLIP_NONE, 0);
		}
	}*/

	if (timer.ReadSec() >= 5 && !timerPaused)
	{
		timerPaused = true;
	}

	if (app->scene->player->vida == 1)
	{
		app->render->DrawTexture(corazon1, 20, 20, NULL, SDL_FLIP_NONE, 0);

	}
	if (app->scene->player->vida == 2)
	{
		app->render->DrawTexture(corazon2, 20, 20, NULL, SDL_FLIP_NONE, 0);

	}
	if (app->scene->player->vida == 3)
	{
		app->render->DrawTexture(corazon3, 20, 20, NULL, SDL_FLIP_NONE, 0);

	}

	//char buffer[20];  // Suficientemente grande para almacenar el entero como cadena
	//snprintf(buffer, sizeof(buffer), "%d", app->scene->player->vida);

	//const char* miVariable = buffer;

	//app->render->DrawText(miVariable, 60, 25, 35, 35);

	return true;

}

// Called each loop iteration
bool SceneGUI::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneGUI::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

//bool Scene::OnGuiMouseClickEvent(GuiControl* control)
//{
//	// L15: TODO 5: Implement the OnGuiMouseClickEvent method
//	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
//	LOG("Press Gui Control: %d", control->id);
//
//	return true;
//}
