#include "Bat.h"

Bat::Bat(int x, int y, bool isQuadtreeNode) : BaseObject(BAT)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ENEMY);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ENEMY, "blue_bat_1"));
	_sprite->setPosition(x, y);

	_animation = new Animation(_sprite, 0.2f);
	_animation->addFrameRect(eID::ENEMY, "blue_bat_2", "blue_bat_3", "blue_bat_4", NULL);

	_effect = SpriteManager::getInstance()->getSprite(eID::EFFECT);
	_effect->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EFFECT, "hit_effect_1"));
	_effectAnimation = new Animation(_effect, 0.15f);
	_effectAnimation->addFrameRect(EFFECT, "hit_effect_1", "hit_effect_2", "hit_effect_3", "hit_effect_4", NULL);

	_isDead = false;
	_isActive = false;
	_initX = x;
	setQuadTree(isQuadtreeNode);
}

void Bat::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_isDead)
		_effectAnimation->draw(spriteHandle, viewport);
	else
	{
		if (_isActive)
			_animation->draw(spriteHandle, viewport);
		else
			_sprite->render(spriteHandle, viewport);
	}
}

void Bat::update(float deltatime)
{
	if (this->getPositionX() < _initX - 520 || this->getPositionX() > _initX + 520)
	{
		this->setStatus(DESTROY);
		return;
	}
	if (!_isDead)
	{
		if (_isActive)
		{
			_animation->update(deltatime);
			if (_effectStopWatch->isTimeLoop(500))
			{
				auto move = (Movement*)this->_componentList["Movement"];
				up = !up;
				if (up)
				{
					move->setVelocity(GVector2(-BAT_SPEED, 50));
				}
				else {
					move->setVelocity(GVector2(-BAT_SPEED, -50));
				}
				_effectStopWatch->restart();
			}
		}
			
		
	}
	else
	{
		_effect->setPosition(this->getPosition());
		_effectAnimation->update(deltatime);
		if (_effectStopWatch->isStopWatch(800))
		{
			this->setStatus(DESTROY);
			srand(time(0));
			auto ran = rand() % 10;
			BaseObject* heart = nullptr;
			if (ran < 3)
				heart = new Heart(this->getPositionX(), this->getPositionY());

			if (heart != nullptr)
			{
				heart->init();
				QuadTreeNode::getInstance()->Insert(heart);
			}
		}
	}

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}

void Bat::wasHit()
{
	_isDead = true;

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));
	_effectStopWatch->isTimeLoop(600);
}

void Bat::release()
{
	SAFE_DELETE(_animation);
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();
}

void Bat::init()
{
	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;

	auto movement = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList["Movement"] = movement;

	_effectStopWatch = new StopWatch();
}

void Bat::Active(bool direct)
{
	_isActive = true;

	auto move = (Movement*)this->_componentList["Movement"];

	if (direct)
	{
		this->setScaleX(-1);
		move->setVelocity(GVector2(BAT_SPEED, -50));
		move->setAccelerate(GVector2(100, 5));
	}
	else
	{
		move->setVelocity(GVector2(-BAT_SPEED, -50));
		move->setAccelerate(GVector2(100, 5));
	}
}


bool Bat::isActive()
{
	return _isActive;
}

bool Bat::isDead()
{
	return _isDead;
}