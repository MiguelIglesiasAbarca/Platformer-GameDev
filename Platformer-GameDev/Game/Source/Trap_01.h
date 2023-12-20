#ifndef __TRAP_H__
#define __TRAP_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Trap : public Entity
{
public:

	Trap();
	virtual ~Trap();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
};

#endif