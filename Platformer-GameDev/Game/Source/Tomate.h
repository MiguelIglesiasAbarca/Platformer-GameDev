#ifndef __TOMATE_H__
#define __TOMATE_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Tomate : public Entity
{
public:

	Tomate();
	virtual ~Tomate();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
};

#endif // __ITEM_H__