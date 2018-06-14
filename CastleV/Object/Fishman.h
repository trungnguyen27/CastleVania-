#ifndef _FISHMAN_H
#define _FISHMAN_H

#include "..\FrameWork\define.h"
#include "BaseObject.h"
#include "..\Framework\IComponent.h"
#include "CollisionBody.h"
#include "../FrameWork/Animation.h"
#include "../Framework/StopWatch.h"
#include "../Framework/SoundManager.h"
#include "QuadtreeNode.h"
#include "Splash.h"
#include <ctime>

#define FISHMAN_SPEED 120.0f
#define GRAVITY 800

class Fishman : public BaseObject {
public:
	Fishman(int x, int y, bool isLeft);

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
	void standing();
	void Active(bool direct);
	bool isActive();
	bool isDead();
	bool isLeft();
	void splash();
protected:
	Animation* _animation;
	map<string, IComponent*> _componentList;
	float _moveSpeed;
	float _initX;
	bool _isDead;
	bool _isActive = false;
	bool _isLeft;
	Sprite* _effect;
	BaseObject* preWall;
	Animation* _effectAnimation;
	StopWatch* _effectStopWatch;
	StopWatch* _jumpStopWatch;
};

#endif