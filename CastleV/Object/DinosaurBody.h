#ifndef _DINOSAURBODY_H_
#define _DINOSAURBODY_H_

#include "..\FrameWork\define.h"
#include "BaseObject.h"
#include "..\Framework\IComponent.h"
#include "CollisionBody.h"
#include "../FrameWork/Animation.h"
#include "../Framework/StopWatch.h"
#include "QuadtreeNode.h"
#include "FireBall.h"
#include <ctime>
#include "BigHeart.h"
#include "Heart.h"
#define SHOOT_DELAY 4500

class DinosaurBody : public BaseObject
{
public:
	DinosaurBody(int x, int y);

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	
	void setDirect(bool direct);

	void wasHit(int hitpoint);
	void Destroy();
	bool isDead();
protected:

	void shoot();

	Animation* _animation;
	
	map<string, IComponent*> _componentList;
	int _hitPoint;

	Sprite* _effect;
	Animation* _effectAnimation;
	StopWatch* _effectStopWatch;

	bool _direct;
	int _ready;
	StopWatch* _readyStopWatch;
	StopWatch* _shoot1StopWatch;
	StopWatch* _shoot2StopWatch;
	
	StopWatch* _hitStopWatch;
	bool _startHit;
};
#endif