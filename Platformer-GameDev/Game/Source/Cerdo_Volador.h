#ifndef __CERDO_VOLADOR__
#define __CERDO_VOLADOR__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Cerdo_Volador : public Entity
{
public:

	Cerdo_Volador();

	virtual ~Cerdo_Volador();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnDeath();

public:
	float speed = 1.0f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	SDL_Texture* pathTexture = NULL;
	PhysBody* pbody;
	iPoint enemyPosition;
	iPoint playerTilePos;
	int distance;
	bool isDead = false;
	bool looksRight = true;

	Animation* currentAnimation = nullptr;
	Animation idleRight;
	Animation runRight;
	Animation dead;
};

#endif
