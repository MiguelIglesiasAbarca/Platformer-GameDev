#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"


struct SDL_Texture;

class SceneIntro : public Module
{
public:

	SceneIntro();

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


	
	SDL_Texture* fondo4;
	pugi::xml_document configFile;
	pugi::xml_node configNode;
	

	//GuiControlButton* gcButtom;


};

#endif // __SCENEINTRO_H__