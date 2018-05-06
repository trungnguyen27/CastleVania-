#ifndef _MONKEY_H
#define _MONKEY_H

#include "..\FrameWork\define.h"
#include "BaseObject.h"
#include "..\Framework\IComponent.h"
#include "CollisionBody.h"
#include "../FrameWork/Animation.h"
#include "../Framework/StopWatch.h"
#include "QuadtreeNode.h"
#include <ctime>
#include "BigHeart.h"
#include "Heart.h"
#include "FireBall.h"
#include <time.h>

#define MONKEY_MOVE_SPEED 70
#define GRAVITY 500
#define SHOOT_DELAY 1000.0f

class Monkey : public BaseObject
{
public:
	Monkey(int x, int y, int activeX);


	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	void standing();

	void wasHit(int hitpoint);
	float checkCollision(BaseObject*, float);
	bool isDead();
	void jump();
	void turnLeft();
	void turnRight();
	void shoot();

	void updatePlayerLocation(GVector2);

protected:
	Animation* _animation;
	map<string, IComponent*> _componentList;
	int _activeXLeft;
	int _activeXRight;

	int _hitPoint;
	bool _wallCollided;
	Sprite* _effect;
	Animation* _effectAnimation;
	StopWatch* _effectStopWatch;
	BaseObject* preWall;

	StopWatch* _hitStopWatch;
	StopWatch* _shootStopWatch;
	bool _startHit;
	bool _isLeft = true;
	GVector2 _currentPlayerLocation;

};
#endif