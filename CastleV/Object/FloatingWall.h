#ifndef _MOVING_STAIR_H
#define _MOVING_STAIR_H

#include "..\FrameWork\define.h"
#include "BaseObject.h"
#include "..\Framework\IComponent.h"
#include "CollisionBody.h"
#include "../FrameWork/Animation.h"
#include "../Framework/StopWatch.h"

#define WALL_MOVE_SPEED 75

class FloatingWall : public BaseObject
{
public:
	FloatingWall(int x, int y, int activeX, bool reversed);


	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	GVector2 getVelocity() override;

protected:

	map<string, IComponent*> _componentList;
	int _activeXLeft;
	int _activeXRight;
	bool _reversed;
};
#endif