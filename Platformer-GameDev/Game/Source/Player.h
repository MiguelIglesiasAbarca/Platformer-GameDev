#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Food.h"
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

	bool Start() override;

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Reset();

	void OnDeath();

	Food* coin;

public:
	float speed = 7.0f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	PhysBody* AttackpBody;
	int jump_FXid;
	int death_Fxid;
	int pickFood_FXid;
	int attack_FXid;
	int running_FXid;
	int checkpoint_FXid;
	bool isJumping = false;
	bool running = false;
	bool isDead = false;
	bool isAttacking = false;
	bool looksRight = false;
	bool godMode = false;
	bool runningFX = false;
	bool destroyBody = false;
	int level = 1;
	int vida = 1;
	int foodCounter = 0;

	Animation* currentAnimation = nullptr;
	Animation idleRight;
	Animation idleLeft;
	Animation runRight;
	Animation runLeft;
	Animation attack;
	Animation dead;
	Animation jumpRight;
	Animation jumpLeft;
	Animation attackRight;
	Animation attackLeft;


};

#endif // __PLAYER_H__