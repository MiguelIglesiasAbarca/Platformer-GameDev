#ifndef __CERDO_BOMBA__
#define __CERDO_BOMBA__

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
	int posA;
	int posB;
	bool direction = true;
	bool isFollowingPlayer = false;
	int health = 100;
	int pigExplosion_FXid;
	int pigOink_FXid;
	bool isDead = false;
	bool looksRight = true;

	Animation* currentAnimation = nullptr;
	Animation runRight;
	Animation watifokIn;
	Animation dead;
};

#endif
