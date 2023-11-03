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

	bool isPicked = false;

private:

	SDL_Texture* texture;
	int movement = position.x;
	const char* texturePath;
	PhysBody* pbody;
};

#endif // __ITEM_H__