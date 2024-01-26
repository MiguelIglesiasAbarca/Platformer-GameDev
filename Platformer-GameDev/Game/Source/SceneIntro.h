#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"
#include "GuiControlButton.h"



struct SDL_Texture;

class SceneIntro : public Module
{
public:

	SceneIntro(App* app, bool start_enabled = true);

	// Destructor
	virtual ~SceneIntro();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

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

	/*bool OnGuiMouseClickEvent(GuiControl* control);*/

public:

	

private:


	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	int scale = 5;
	
	
	SDL_Texture* gatitorico;

	
	pugi::xml_document configFile;
	pugi::xml_node configNode;
	
	Timer timer;
	//GuiControlButton* gcButtom;

	bool timerPaused = false;

	SDL_Texture* background;
	SDL_Texture* menu;
	SDL_Texture* playHover;
	SDL_Texture* playClick;
	SDL_Texture* continueHover;
	SDL_Texture* continueClick;
	SDL_Texture* settingsHover;
	SDL_Texture* settingsClick;
	SDL_Texture* creditsHover;
	SDL_Texture* creditsClick;
	SDL_Texture* exitHover;
	SDL_Texture* exitClick;

	GuiControlButton* playButton;
	GuiControlButton* continueButton;
	GuiControlButton* settingsButton;
	GuiControlButton* creditsButton;
	GuiControlButton* exitButton;
};

#endif // __SCENEINTRO_H__