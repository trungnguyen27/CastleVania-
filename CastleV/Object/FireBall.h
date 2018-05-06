#ifndef _FIREBALL_H_
#define _FIREBALL_H_

#include "..\FrameWork\define.h"
#include "BaseObject.h"
#include "..\Framework\IComponent.h"
#include "CollisionBody.h"
#include "../FrameWork/Animation.h"
#include "../Framework/StopWatch.h"

#define FIREBALL_SPEED 140

class FireBall : public BaseObject
{
public:
	FireBall(int x, int y, bool direct);

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	void wasHit();
	bool isDead();
	void setDirection(GVector2);
protected:
	map<string, IComponent*> _componentList;

	bool _isDead;

	Sprite* _effect;
	Animation* _effectAnimation;
	StopWatch* _effectStopWatch;
};
#endif