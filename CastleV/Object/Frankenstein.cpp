#include "Frankenstein.h"

Frankenstein::Frankenstein(int x, int y) : BaseObject(FRANKENSTEIN)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ENEMY);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ENEMY, "frankenstein_1"));
	_sprite->setPosition(x, y);
	_sprite->setScale(1.5f);

	_animation = new Animation(_sprite, 0.2f);
	_animation->addFrameRect(eID::ENEMY, "frankenstein_1", "frankenstein_2", "frankenstein_3", NULL);

	_isDead = false;
	_isActive = false;
	_initX = x;
}

void Frankenstein::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_isDead)
		_effectAnimation->draw(spriteHandle, viewport);
	else
	{
		_animation->draw(spriteHandle, viewport);
	}
}

void Frankenstein::update(float deltatime)
{
	if (!_isDead)
	{
		if (_isActive)
		{
			
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
			srand(time(0));
			auto ran = rand() % 10;
			BaseObject* item = nullptr;
			//if (ran < 3)
			//	item = new BigHeart(this->getPositionX(), this->getPositionY());
			//else if (ran > 6)
			//	item = new Money(this->getPositionX(), this->getPositionY(), ran - 7);
			//else
			//	item = new Heart(this->getPositionX(), this->getPositionY());
			//if (item != nullptr)
			//{
			//	item->init();
			//	QuadTreeNode::getInstance()->Insert(item);
			//}
		}
		

	}

	this->updateStatus(deltatime);
	_animation->update(deltatime);

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

	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;
	
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

void Frankenstein::moveToPlayer(int direction)
{
	if (_currentDirection != direction)
	{
		_currentDirection = direction;
	}
	else return;

	this->setScaleX(abs(this->getScale().x) * _currentDirection);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(_currentDirection*_moveSpeed * -1, move->getVelocity().y));
}

void Frankenstein::standing()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));
	this->removeStatus(BEING_HIT);
	this->removeStatus(eStatus::JUMPING);
	this->removeStatus(eStatus::FALLING);
}

void Frankenstein::release()
{
	SAFE_DELETE(_animation);
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();
}

GVector2 Frankenstein::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
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
	_jumpStopWatch = new StopWatch();
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