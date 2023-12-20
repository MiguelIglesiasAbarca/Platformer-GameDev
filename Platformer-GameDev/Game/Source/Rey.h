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

public:
	const char* texturePath;
	SDL_Texture* texture = NULL;
	int king_FXid;
	PhysBody* pbody;
};

#endif
