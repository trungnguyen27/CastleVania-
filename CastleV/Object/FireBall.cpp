#include "FireBall.h"

FireBall::FireBall(int x, int y, bool direct) :BaseObject(FIREBALL)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ENEMY);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ENEMY, "fireball"));
	_sprite->setPosition(x, y);
	
	_effect = SpriteManager::getInstance()->getSprite(eID::EFFECT);
	_effect->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EFFECT, "hit_effect_1"));
	_effectAnimation = new Animation(_effect, 0.15f);
	_effectAnimation->addFrameRect(EFFECT, "hit_effect_1", "hit_effect_2", "hit_effect_3", "hit_effect_4", NULL);

	_isDead = false;
}

void FireBall::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (!_isDead)
	{
		_sprite->render(spriteHandle, viewport);

		if (!isIntersectedInGame(viewport->getBounding(), _sprite->getBounding()))
		{
			this->setStatus(DESTROY);
		}
	}
	else
		_effectAnimation->draw(spriteHandle, viewport);
}

void FireBall::setDirection(GVector2 dir)
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(dir.x * FIREBALL_SPEED, dir.y * FIREBALL_SPEED));
}

void FireBall::update(float deltatime)
{
	if (_isDead)
	{
		_effectAnimation->update(deltatime);
		if (_effectStopWatch->isStopWatch(600))
		{
			this->setStatus(DESTROY);
		}
	}

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}

void FireBall::release()
{
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();
}

void FireBall::init()
{
	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;
	auto movement = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList["Movement"] = movement;
	_effectStopWatch = new StopWatch();
}

bool FireBall::isDead()
{
	return _isDead;
}

void FireBall::wasHit()
{
	_isDead = true;
	_effect->setPosition(_sprite->getPosition());
	_effectStopWatch->isStopWatch(600);
}

