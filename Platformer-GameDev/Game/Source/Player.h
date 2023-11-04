#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnDeath();

public:
	float speed = 7.0f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int pickCoinFxId;
	bool isJumping = false;
	bool running = false;
	bool isDead = false;

	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation Runright;
	Animation Runleft;
	Animation Attack;
	Animation Dead;
	Animation Jump;


};

#endif // __PLAYER_H__