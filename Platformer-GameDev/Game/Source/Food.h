#ifndef __FOOD__
#define __FOOD__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Food : public Entity
{
public:

	Food();
	virtual ~Food();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void Food::OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	int movement = position.x;
	const char* texturePath;
	PhysBody* pbody;
};

#endif // __ITEM_H__