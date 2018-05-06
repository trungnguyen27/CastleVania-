#ifndef _SPLASH_H_
#define _SPLASH_H_

#include "..\FrameWork\define.h"
#include "BaseObject.h"
#include "..\Framework\IComponent.h"
#include "CollisionBody.h"
#include "../FrameWork/Animation.h"
#include "../Framework/StopWatch.h"

class Splash : public BaseObject
{
public:
	Splash(int x, int y);

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	void doSplash(GVector2 direction);
	RECT getBounding() override;

	float checkCollision(BaseObject* object, float dt) override;
protected:
	map<string, IComponent*> _componentList;
	bool _stop;
	int _initX;
	bool _splash = false;
	StopWatch* _destroyStopWatch;
	bool _startDestroyStopWatch;
};
#endif