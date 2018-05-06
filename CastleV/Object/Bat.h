#ifndef _BAT_H
#define _BAT_H

#include "..\FrameWork\define.h"
#include "BaseObject.h"
#include "..\Framework\IComponent.h"
#include "CollisionBody.h"
#include "../FrameWork/Animation.h"
#include "../Framework/StopWatch.h"
#include "QuadtreeNode.h"
#include <ctime>

#define BAT_SPEED 120

class Bat : public BaseObject {
public:
	Bat(int x, int y);

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	void Active(bool direct);
	bool isActive();
	bool isDead();
protected:
	Animation* _animation;
	map<string, IComponent*> _componentList;
	float _initX;
	bool _isDead;

	bool _isActive;
	bool up;

	Sprite* _effect;
	Animation* _effectAnimation;
	StopWatch* _effectStopWatch;
};

#endif