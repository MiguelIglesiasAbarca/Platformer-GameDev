#ifndef __DRAGON__
#define __DRAGON__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Dragon : public Entity
{
public:

	Dragon();

	virtual ~Dragon();

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
	bool direction = true;
	bool isFollowingPlayer = false;
	int health = 100;
	int distance;
	bool isDead = false;
	//bool isJumping = false;
	//bool running = false;
	//bool isDead = false;
	bool looksRight = true;

	Animation* currentAnimation = nullptr;
	Animation idleRight;
	Animation idleLeft;
	Animation runRight;
	Animation runLeft;
	//Animation attack;
	Animation dead;
	//Animation jumpRight;
	//Animation jumpLeft;
};

#endif


