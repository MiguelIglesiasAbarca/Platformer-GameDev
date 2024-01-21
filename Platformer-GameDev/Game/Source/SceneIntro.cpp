#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneIntro.h"


#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"

SceneIntro::SceneIntro(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("scene");
}

// Destructor
SceneIntro::~SceneIntro()
{}

// Called before render is available
bool SceneIntro::Awake(pugi::xml_node& config)
{
	configNode = config;
	LOG("Loading SceneIntro");
	bool ret = true;

	return ret;
}

// Called before Fthe first frame
bool SceneIntro::Start()
{
	LOG("Loading SceneIntro Assets");
	bool ret = true;

	gatitorico = app->tex->Load("Assets/Textures/gatoguapo.png");
	app->win->GetWindowSize(windowW, windowH);

	app->audio->PlayMusic("Assets/Audio/Music/background_music.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	return true;
}

// Called each loop iteration
bool SceneIntro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneIntro::Update(float dt)
{
	//dibujamos
	SDL_Rect RectFondoInicial{ 0, 0, windowW-30, windowH-30 };
	app->render->DrawTexture(gatitorico, 830, 890, &RectFondoInicial);
	//if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool SceneIntro::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneIntro::CleanUp()
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
