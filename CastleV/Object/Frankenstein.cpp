#include "Frankenstein.h"

Frankenstein::Frankenstein(int x, int y, int checkpoint, int ballPos, bool quadtree) : BaseObject(FRANKENSTEIN)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ENEMY);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ENEMY, "frankenstein_1"));
	_sprite->setPosition(x, y);
	_sprite->setScale(1.25f);

	_monkey = new Monkey(x, y+65, 200);
	_monkey->init();
	_monkey->follow(this);

	_animation = new Animation(_sprite, 0.2f);
	_animation->addFrameRect(eID::ENEMY, "frankenstein_1", "frankenstein_2", "frankenstein_3", NULL);
	_animation->setValueFlashes(0.5f);

	_effect = SpriteManager::getInstance()->getSprite(eID::EFFECT);
	_effect->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EFFECT, "fire_1"));
	_effectAnimation = new Animation(_effect, 0.15f);
	_effectAnimation->addFrameRect(EFFECT, "fire_1", "fire_2", "fire_3", "fire_4", NULL);

	_isDead = false;
	_isActive = false;
	_initX = x;
	_hitpoint = 16;
	_ballPosition = ballPos;
	_checkpoint = checkpoint;
	setQuadTree(quadtree);
}

void Frankenstein::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_isDead) {
		_effectAnimation->draw(spriteHandle, viewport);
	}
	else
	{
		_monkey->draw(spriteHandle, viewport);
		_animation->draw(spriteHandle, viewport);
		
	}
}

void Frankenstein::update(float deltatime)
{
	if (_hitpoint <= 0 && !_isDead)
	{
		_isDead = true;
		auto move = (Movement*)this->_componentList["Movement"];
		move->setVelocity(GVector2(0, 0));
		_effectStopWatch->isTimeLoop(800);
	}

	if (!_isDead)
	{
		if (_isActive)
		{
			if (_beHit)
			{
				if (_hitStopWatch->isStopWatch(1000))
				{
					_beHit = false;
					_animation->enableFlashes(false);
				}
				
			}
			else
				this->moveToPlayer();
		}
	/*	if (_jumpStopWatch->isStopWatch(500))
		{
			this->setStatus(eStatus::FALLING);
		}*/
	}
	else
	{
		_effect->setPosition(this->getPosition());
		_effectAnimation->update(deltatime);
		if (_effectStopWatch->isStopWatch(800))
		{
			this->setStatus(DESTROY);
			auto viewportPos = SceneManager::getInstance()->getCurrentScene()->getViewport()->getPositionWorld();
			auto item = new Ball(viewportPos.x + 256, viewportPos.y - 200, MAP_STAGE_12);
			item->init();
			QuadTreeNode::getInstance()->Insert(item);
		}
	}

	this->updateStatus(deltatime);
	_animation->update(deltatime);
	_monkey->update(deltatime);

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}

float Frankenstein::checkCollision(BaseObject* object, float dt)
{
	if (object->getStatus() == eStatus::DESTROY || this->isInStatus(eStatus::DIE))
		return 0.0f;
	if (this == object)
		return 0.0f;

	if (_monkey)
		_monkey->checkCollision(object, dt);

	if (object->getStatus() == eStatus::DESTROY || this->isInStatus(eStatus::DIE))
		return 0.0f;
	if (this == object)
		return 0.0f;

	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;
	if ( objectId == MONKEY_WALL)
	{
		if (collisionBody->checkCollision(object, direction, dt, false) && !this->isInStatus(MOVING_UP))
		{
			float moveX, moveY;
			if (collisionBody->isColliding(object, moveX, moveY, dt))
			{
				collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
				_beHit = false;
				this->moveToPlayer();
			}
		}
	}
	_monkey->checkCollision(object, dt);
}

void Frankenstein::jump()
{
	if (this->isInStatus(eStatus::JUMPING) || this->isInStatus(eStatus::FALLING))
		return;

	this->addStatus(eStatus::JUMPING);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(move->getVelocity().x, 700));

	auto g = (Gravity*)this->_componentList["Gravity"];
	g->setStatus(eGravityStatus::FALLING__DOWN);
}

void Frankenstein::updateStatus(float dt)
{
	//if ((this->getStatus() & eStatus::NORMAL) == eStatus::NORMAL && this->isActive() == true)
	//{
	//	auto gravity = (Gravity*)this->_componentList["Gravity"];
	//	if ((gravity->getStatus() & eGravityStatus::SHALLOWED) == eGravityStatus::SHALLOWED)
	//		this->moveToPlayer();
	//}
}



void Frankenstein::moveToPlayer()
{
	int direction = 1;
	if ((_follow->getPositionX() - this->getPositionX() > 0))
		direction = -1;
	else direction = 1;

	this->setScaleX(abs(this->getScale().x) * direction);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(direction*_moveSpeed * -1, move->getVelocity().y));
}

void Frankenstein::moveAwayFromPlayer()
{
	int direction = 1;
	if ((_follow->getPositionX() - this->getPositionX() > 0))
		direction = -1;
	else direction = 1;

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(direction*_moveSpeed, move->getVelocity().y));
}

void Frankenstein::follow(BaseObject* follow) {
	if (_follow != follow)
	{
		_follow = follow;
	}
}

void Frankenstein::releaseMonkey()
{
	if (_monkey)
	{
		_monkey->follow(NULL);
		_monkey->setShootingTarget(_follow);
		_monkey->active(true);
	}
}

void Frankenstein::standing()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));
	this->removeStatus(BEING_HIT);
	this->removeStatus(eStatus::JUMPING);
	this->removeStatus(eStatus::FALLING);
}

void Frankenstein::beHit()
{
	if (!_beHit)
	{
		_beHit = true;
		_hitStopWatch->restart();
		_hitpoint -= 2;
		_animation->enableFlashes(true);
		this->moveAwayFromPlayer();
	}
}

void Frankenstein::release()
{
	SAFE_DELETE(_animation);
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();
	SAFE_DELETE(_monkey);
}

GVector2 Frankenstein::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}

int Frankenstein::getHitPoint()
{
	return _hitpoint;
}

int Frankenstein::getCheckpoint()
{
	return _checkpoint;
}

int Frankenstein::getBallPosition()
{
	return _ballPosition;
}

void Frankenstein::init()
{
	_moveSpeed = FRANKENSTEIN_SPEED;
	_isDead = false;
	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;

	auto movement = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList["Movement"] = movement;

	_componentList["Gravity"] = new Gravity(GVector2(0, -GRAVITY), movement);
	this->setOrigin(GVector2(0.5f, 0.0f));
	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::SHALLOWED);

	_effectStopWatch = new StopWatch();
	_hitStopWatch = new StopWatch();

}

void Frankenstein::Active(bool active)
{
	_isActive = active;
}


bool Frankenstein::isActive()
{
	return _isActive;
}

bool Frankenstein::isDead()
{
	return _isDead;
}


void Frankenstein::setStatus(eStatus status)
{
	_status = status;
}