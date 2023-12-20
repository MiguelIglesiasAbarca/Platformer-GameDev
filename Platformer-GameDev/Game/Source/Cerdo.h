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

	void Move(const iPoint& origin, const iPoint& destination);

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 1.0f;
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
