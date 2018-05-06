#ifndef _DINOSAUR_H_
#define _DINOSAUR_H_

#include "..\FrameWork\define.h"
#include "BaseObject.h"
#include "..\Framework\IComponent.h"
#include "CollisionBody.h"
#include "../FrameWork/Animation.h"
#include "../Framework/StopWatch.h"
#include "QuadtreeNode.h"
#include "FireBall.h"
#include "DinosaurBody.h"
#include <ctime>
#include "BigHeart.h"
#include "Heart.h"
#define SHOOT_DELAY 4500
#define MOVE_SPEED 50
#define MAX_DISTANCE_FORWAD 100
#define BODY_LENGTH 90
#define UP_DISTANCE 100
#define DOWN_DISTANCE 100
#define BODY_PARTS 7
enum DinosaurStatus {
	IDLE = 0,
	FORWARD = 1,
	UP = 2,
	DOWN = 3
};

class Dinosaur : public BaseObject
{
public:
	Dinosaur(int x, int y);

	void init() override;
	void update(float deltatime) override;
	void updateStatus(float deltatime);
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	
	void goUp(float);
	void goDown(float);
	void goForward(GVector2 direction);
	void move(float deltaTime);
	void stop();
	void setDirect(bool direct);

	DinosaurStatus getStatus() { return _status; }

	void wasHit(int hitpoint);
	void Active(bool active);
	bool isActive() { return _active; }
	bool isDead();
protected:

	DinosaurStatus _status;
	GVector2 _headInitialPos;
	GVector2 _bodyInitialPos;
	GVector2 _initialVelocity;
	float _lastYPos = 0;
	float elapsedTime = 0;
	bool _active;
	void shoot();

	Animation* _animation;
	
	map<string, IComponent*> _componentList;
	vector<BaseObject*> bodyParts;
	int _hitPoint;

	Sprite* _effect;
	Animation* _effectAnimation;
	StopWatch* _effectStopWatch;

	bool _direct;
	int _ready;
	StopWatch* _readyStopWatch;
	StopWatch* _goForwardStopWatch;
	StopWatch* _shoot1StopWatch;
	StopWatch* _shoot2StopWatch;
	
	StopWatch* _hitStopWatch;
	bool _startHit;
};
#endif