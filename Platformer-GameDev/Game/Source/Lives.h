#ifndef __LIVES_H__
#define __LIVES_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Lives : public Entity
{
public:

    Lives();
    virtual ~Lives();

    bool Awake();

    bool Start();

    bool Update(float dt);

    bool CleanUp();

    void Lives::OnCollision(PhysBody* physA, PhysBody* physB);

public:

    bool isPicked = false;

private:

    int pickLives_FXid;
    SDL_Texture* texture;
    int movement = position.x;
    const char* texturePath;
    PhysBody* pbody;
};

#endif // __LIVES_H__
