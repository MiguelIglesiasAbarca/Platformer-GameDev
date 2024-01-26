#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Cerdo.h"
#include "Cerdo_bomba.h"
#include "Dragon.h"
#include "Cerdo_Volador.h"
#include "Food.h"
#include "Rey.h"
#include "Boss.h"
#include "Item.h"
#include "Trap_01.h"
#include "Lives.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Scene();

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

	bool LoadState(pugi::xml_node node);

	//
	bool SaveState(pugi::xml_node node);

	bool OnGuiMouseClickEvent(GuiControl* control);

public:

	Player* player;

private:

	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	int scale = 5;
	

	SDL_Texture* fondo0;
	SDL_Texture* fondo1;
	SDL_Texture* fondo2;
	SDL_Texture* fondo3;
	SDL_Texture* fondo4;
	SDL_Texture* hearts_tex;
	pugi::xml_document configFile;
	pugi::xml_node configNode;
	List<Entity*> Cerdolista;
	List<Entity*> CerdoVoladorlista;
	List<Entity*> BossLista;
	List<Entity*> CerdoPatrulladorlista;
	List<Entity*> Dragonlista;

	GuiControlButton* gcButtom;


};

#endif // __SCENE_H__