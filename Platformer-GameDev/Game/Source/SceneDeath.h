#ifndef __SCENEDEATH_H__
#define __SCENEDEATH_H__

#include "Module.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"

struct SDL_Texture;

class SceneDeath : public Module
{
public:

	SceneDeath(App* application, bool start_enabled = true);

	// Destructor
	virtual ~SceneDeath();

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
	

	SDL_Texture* currentTexture;

	GuiControlButton* btn1;
	GuiControlButton* btn2;



	int count;

	// Screen music
	uint logo_theme;

	pugi::xml_node node;

};

#endif // __SCENEDEATH_H__