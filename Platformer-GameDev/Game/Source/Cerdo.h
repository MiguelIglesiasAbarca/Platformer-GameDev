#ifndef __CERDO__
#define __CERDO__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Cerdo : public Entity
{
public:

	Cerdo();

	virtual ~Cerdo();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	//void OnDeath();

public:
	float speed = 7.0f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	SDL_Texture* pathTexture = NULL;
	PhysBody* pbody;
	iPoint playerTilePos;
	//bool isJumping = false;
	//bool running = false;
	//bool isDead = false;
	//bool left_right = true;

	Animation* currentAnimation = nullptr;
	//Animation idleRight;
	//Animation idleLeft;
	//Animation runRight;
	//Animation runLeft;
	//Animation attack;
	//Animation dead;
	//Animation jumpRight;
	//Animation jumpLeft;
};

#endif
