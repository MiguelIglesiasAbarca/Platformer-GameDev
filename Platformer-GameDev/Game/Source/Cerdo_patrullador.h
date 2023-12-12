#ifndef __CERDO_PATRULLADOR__
#define __CERDO_PATRULLADOR__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class CerdoPatrullador : public Entity
{
public:

	CerdoPatrullador();

	virtual ~CerdoPatrullador();

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
	iPoint cerdoPosition;
	iPoint playerTilePos;
	int posA;
	int posB;
	bool direction = true;
	int health = 100;
	//bool isJumping = false;
	//bool running = false;
	//bool isDead = false;
	//bool left_right = true;

	Animation* currentAnimation = nullptr;
	/*Animation idleRight;*/
	//Animation idleLeft;
	Animation runRight;
	Animation runLeft;
	//Animation attack;
	Animation dead;
	//Animation jumpRight;
	//Animation jumpLeft;
};

#endif
