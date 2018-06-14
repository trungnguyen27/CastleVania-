#include "Splash.h"


Splash::Splash(int x, int y) : BaseObject(SPLASH)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::EFFECT);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EFFECT, "splash_effect"));
	_sprite->setPosition(x, y);
	_initX = x;
	_stop = false;
}

void Splash::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	_sprite->render(spriteHandle, viewport);
}

void Splash::update(float deltatime)
{
	if (this->getPositionY() < -100)
		this->setStatus(DESTROY);
}

void Splash::release()
{
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();
}

void Splash::init()
{
	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;

	auto movement = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList["Movement"] = movement;

	_destroyStopWatch = new StopWatch();
	_startDestroyStopWatch = false;

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(200, 300));
	move->setAccelerate(GVector2(-100, -800));
}

RECT Splash::getBounding()
{
	return _sprite->getBounding();
}

void Splash::doSplash(GVector2 direction)
{
	
	_splash = true;
}


float Splash::checkCollision(BaseObject* object, float dt)
{
	/*if (object->getId() == WALL)
	{
		auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
		eDirection direction;
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			if (direction == TOP)
			{
				float moveX, moveY;
				if (collisionBody->isColliding(object, moveX, moveY, dt))
				{
					collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
				}
				if (!_startDestroyStopWatch)
				{
					_destroyStopWatch->isTimeLoop(2000);
					_startDestroyStopWatch = true;
				}
				_stop = true;
			}
			return 1.0f;
		}
	}*/
	return 0;
}