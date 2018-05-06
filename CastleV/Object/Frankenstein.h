#ifndef _FRANKENSTEIN_H
#define _FRANKENSTEIN_H

#include "..\FrameWork\define.h"
#include "BaseObject.h"
#include "Monkey.h"
#include "..\Framework\IComponent.h"
#include "CollisionBody.h"
#include "../FrameWork/Animation.h"
#include "../Framework/StopWatch.h"
#include "QuadtreeNode.h"
#include <ctime>

#define FRANKENSTEIN_SPEED 30.0f
#define GRAVITY 800

class Frankenstein : public BaseObject {
public:
	Frankenstein(int x, int y);

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	void updateStatus(float dt);
	void setStatus(eStatus status) override;
	float checkCollision(BaseObject* object, float dt);
	GVector2 getVelocity();
	void jump();
	void moveToPlayer();
	void moveAwayFromPlayer();
	void standing();
	void beHit();
	void releaseMonkey();
	void follow(BaseObject* );
	void Active(bool active);
	bool isActive();
	bool isDead();
protected:
	Monkey * _monkey;

	Animation* _animation;
	map<string, IComponent*> _componentList;
	float _moveSpeed;
	float _initX;
	bool _isDead;
	bool _isActive = false;
	bool _beHit;
	Sprite* _effect;
	BaseObject* preWall;
	BaseObject* _follow;
	Animation* _effectAnimation;
	StopWatch* _effectStopWatch;
	StopWatch* _hitStopWatch;
};

#endif