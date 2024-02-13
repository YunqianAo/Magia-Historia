#pragma once
#ifndef __ENEMY_GOBLIN_H__
#define __ENEMY_GOBLIN_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include <vector>


struct SDL_Texture;

class Enemy_Goblin : public Entity
{
public:

	Enemy_Goblin();

	virtual ~Enemy_Goblin();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();


	void EnemyMove(float dt, int enemyAreaLimitL, int enemyAreaLimitR);
	bool RandSelecion();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnEndCollision(PhysBody* physA, PhysBody* physB);

	void reLoadXML();

public:

	bool isPicked = false;

	Animation* currentAnimation = nullptr;

	Animation idle;
	Animation atack;
	Animation die;
	Animation take_hit;
	Animation run;

	Animation SPosition;
	SDL_Rect* spritePositions;

	int TSprite;
	int SpriteX;
	int SpriteY;
	int PhotoWeight;
	int countFrame = 0;
	float speed;

	int lasPosX;
	bool isFacingLeft = true;
	b2Vec2 vel;
	b2Filter filter;

	bool AtackPlayer = false;

	int walkFrameCount = 0;
	bool rddirection;
	int enemywalk;
	bool enemyidle = true;
	int timeidle;
	bool rdinWork = false;
	bool walkrdinWork = true;
	int enemyAreaLimitR, enemyAreaLimitL;

	bool inEenemyArea = true;
	int enemyOutAreaTime;
	
	int life;
	bool isDead = false;
	bool isTakehit;
	bool canatake = true;
	bool canatake_speed = true;
	int atakeSpeed = 0;

	bool isDestroyPar = false;


	bool atacando = false;
	Timer timerAtaque;
	Timer attackTimer;
	PhysBody* attackParticle;

	int deadenemy;

	float rangeSize = 700.0f;
	float leftTopX;
	float leftTopY;
	float rightBottomX;
	float rightBottomY;


	float atk_rangeSize = 40.0f;
	float atk_leftTopX;
	float atk_leftTopY;
	float atk_rightBottomX;
	float atk_rightBottomY;

	Player* player;

	int i = 0;
	int j = 0;

private:

	

	SDL_Texture* Enemytexture;
	const char* EnemyPath;
	PhysBody* pbody;
};

#endif // __ITEM_H__