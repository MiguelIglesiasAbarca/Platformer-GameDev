#ifndef __ENEMY_A_H__
#define __ENEMY_A_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

class Enemy_A : public Entity
{
public:

	Enemy_A();

	virtual ~Enemy_A();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	//void OnDeath();

public :

	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;

	/*Animation* currentAnimation = nullptr;
	Animation idleRight;
	Animation idleLeft;
	Animation runRight;
	Animation runLeft;
	Animation attack;
	Animation dead;
	Animation jumpRight;
	Animation jumpLeft;*/
};

#endif

