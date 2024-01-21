#ifndef __BOSS__
#define __BOSS__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Boss : public Entity
{
public:

	Boss();

	virtual ~Boss();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void Move(const iPoint& origin, const iPoint& destination);

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Attack();

public:
	float speed;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	SDL_Texture* pathTexture = NULL;
	PhysBody* pbody;
	PhysBody* AttackpBody;
	iPoint enemyPosition;
	iPoint nextTilePath;
	iPoint playerTilePos;
	int distance;
	bool isJumping = false;
	bool hasAttacked = false;
	bool isDead = false;
	bool destroyBody = false;
	bool isAttacking = false;
	bool looksRight = true;

	Animation* currentAnimation = nullptr;
	Animation idleRight;
	Animation runRight;
	Animation jumpRight;
	Animation attack;
	Animation dead;
};

#endif
