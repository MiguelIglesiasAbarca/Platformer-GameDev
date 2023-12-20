#ifndef __REY__
#define __REY__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Rey : public Entity
{
public:

	Rey();

	virtual ~Rey();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnDeath();

public:
	//float speed = 1.0f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	//SDL_Texture* pathTexture = NULL;
	int king_FXid;
	PhysBody* pbody;
	//iPoint cerdoPosition;
	//iPoint playerTilePos;
	//int posA;
	//int posB;
	//bool direction = true;
	//bool isFollowingPlayer = false;
	//int health = 100;
	//int pigExplosion_FXid;
	//int pigOink_FXid;
	//bool isJumping = false;
	//bool running = false;
	//bool isDead = false;
	//bool left_right = true;

	//Animation* currentAnimation = nullptr;
	/*Animation idleRight;*/
	//Animation idleLeft;
	//Animation runRight;
	//Animation runLeft;
	//Animation watifokIn;
	//Animation attack;
	//Animation dead;
	//Animation jumpRight;
	//Animation jumpLeft;
};

#endif
