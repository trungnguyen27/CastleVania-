#ifndef _BALL_H
#define _BALL_H

#include "..\FrameWork\define.h"
#include "BaseObject.h"
#include "..\Framework\IComponent.h"
#include "CollisionBody.h"
#include "../FrameWork/Animation.h"
#include "../Framework/StopWatch.h"

class Ball : public BaseObject
{
public:
	Ball(int x, int y, eID id);

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	RECT getBounding() override;

	float checkCollision(BaseObject* object, float dt) override;

	eID GetNext();
protected:
	map<string, IComponent*> _componentList;

	Animation* _animation;

	bool _stop;
	int _initX;

	eID _id;
};
#endif