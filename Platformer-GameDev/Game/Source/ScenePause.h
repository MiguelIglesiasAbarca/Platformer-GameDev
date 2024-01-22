#ifndef __SCENEPAUSE_H__
#define __SCENEPAUSE_H__

#include "Module.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"

struct SDL_Texture;

class ScenePause : public Module
{
public:

	ScenePause(App* application, bool start_enabled = true);

	// Destructor
	virtual ~ScenePause();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// GUI
	bool OnGuiMouseClickEvent(GuiControl* control);

	bool newgame = false;

private:

	// Texture
	//Pause
	SDL_Texture* Pause_1;
	SDL_Texture* Pause_2;
	SDL_Texture* Pause_3;
	SDL_Texture* Pause_4;
	SDL_Texture* Pause_5;
	SDL_Texture* Pause_6;
	SDL_Texture* Pause_7;
	SDL_Texture* Pause_8;
	SDL_Texture* Pause_9;

	SDL_Texture* currentTexture;

	GuiControlButton* btn1;
	GuiControlButton* btn2;
	GuiControlButton* btn3;
	GuiControlButton* btn4;

	int count;

	// Screen music
	uint logo_theme;

	pugi::xml_node node;

};

#endif // __SCENESETTINGS_H__