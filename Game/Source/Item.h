#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"

#include "SDL/include/SDL.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool isPicked = false;
	Animation* currentAnimation = nullptr;
	Animation openBox;

private:

	SDL_Texture* texture;
	const char* texturePath;
	SDL_Texture* Boxtexture;
	const char* BoxPath;
	PhysBody* pbody;
};

#endif // __ITEM_H__