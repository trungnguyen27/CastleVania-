#include "Fishman.h"

Fishman::Fishman(int x, int y, bool isLeft) : BaseObject(FISHMAN)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ENEMY);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ENEMY, "fishman_1"));
	_sprite->setPosition(x, y);

	_animation = new Animation(_sprite, 0.4f);
	_animation->addFrameRect(eID::ENEMY,"fishman_2", "fishman_3", NULL);



	_isDead = false;
	_isActive = false;
	_initX = x;
	_isLeft = isLeft;
}

void Fishman::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_isDead)
		_effectAnimation->draw(spriteHandle, viewport);
	else
	{
		if (_isActive) {
			_animation->draw(spriteHandle, viewport);
		}
	}
}

void Fishman::update(float deltatime)
{
	if (this->getPositionX() < _initX - 520 || this->getPositionX() > _initX + 520)
	{
		SoundManager::getInstance()->Play(FISHMAN_FALL);
		this->setStatus(DESTROY);
		return;
	}
	if (!_isDead)
	{
		if (_isActive)
		{
			if (this->getPositionY() < -20)
			{
				splash();
				SoundManager::getInstance()->Play(FISHMAN_FALL);
				this->setStatus(DESTROY);
			}
		}

	
	/*	if (_jumpStopWatch->isStopWatch(500))
		{
			this->setStatus(eStatus::FALLING);
		}*/
	}
	else
	{
		/*_effect->setPosition(this->getPosition());
		_effectAnimation->update(deltatime);*/
		if (_effectStopWatch->isStopWatch(800))
		{
			this->setStatus(DESTROY);
			srand(time(0));
			auto ran = rand() % 10;
		}
		

	}

	this->updateStatus(deltatime);
	_animation->update(deltatime);

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}


}

float Fishman::checkCollision(BaseObject* object, float dt)
{
	if (object->getStatus() == eStatus::DESTROY || this->isInStatus(eStatus::DIE))
		return 0.0f;
	if (this == object)
		return 0.0f;

	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;
	if (objectId == WALL || objectId == FISHMAN_WALL)
	{
		if (collisionBody->checkCollision(object, direction, dt, false) && !this->isInStatus(MOVING_UP))
		{
			if (direction != BOTTOM)
			{
				if (objectId == FISHMAN_WALL)
				{
					if (direction == RIGHT)
						this->_isLeft = false;
					else if (direction == LEFT)
						return 1.0f;
				}
				float moveX, moveY;
				if (collisionBody->isColliding(object, moveX, moveY, dt))
				{
					collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
				}
			}
			else {
				float moveX, moveY;
				if (collisionBody->isColliding(object, moveX, moveY, dt))
				{
					return 1.0f;
				}
			}


			this->isInStatus(eStatus::JUMPING);

			if (direction == eDirection::TOP)
				if (!(this->getVelocity().y > -200 && this->isInStatus(eStatus::JUMPING)))
				{
					auto gravity = (Gravity*)this->_componentList["Gravity"];
					gravity->setStatus(eGravityStatus::SHALLOWED);

					this->standing();
					preWall = object;
				}
		}
		else if (preWall == object)
		{
			auto gravity = (Gravity*)this->_componentList["Gravity"];

			if (!this->isInStatus(eStatus::MOVING_DOWN)
				&& !this->isInStatus(eStatus::MOVING_UP)
				&& !this->isInStatus(eStatus::STAND_DOWN)
				&& !this->isInStatus(eStatus::STAND_UP))
			{
				gravity->setStatus(eGravityStatus::FALLING__DOWN);
				if (!this->isInStatus(eStatus::JUMPING) && !this->isInStatus(eStatus::FALLING) && !this->isInStatus(eStatus::BEING_HIT))
				{
					this->addStatus(eStatus::FALLING);
					auto move = (Movement*)this->_componentList["Movement"];
					auto vec = move->getVelocity();
					vec.x = 0;
					move->setVelocity(vec);
				}
			}
		}
	}
}

void Fishman::jump()
{
	if (this->isInStatus(eStatus::JUMPING) || this->isInStatus(eStatus::FALLING))
		return;

	this->addStatus(eStatus::JUMPING);

	SoundManager::getInstance()->Play(FISHMAN_JUMP);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(move->getVelocity().x, 700));

	auto g = (Gravity*)this->_componentList["Gravity"];
	g->setStatus(eGravityStatus::FALLING__DOWN);
}

void Fishman::updateStatus(float dt)
{
	if ((this->getStatus() & eStatus::NORMAL) == eStatus::NORMAL && this->isActive() == true)
	{
		auto gravity = (Gravity*)this->_componentList["Gravity"];
		if ((gravity->getStatus() & eGravityStatus::SHALLOWED) == eGravityStatus::SHALLOWED)
			this->moveToPlayer();
	}
}

void Fishman::moveToPlayer()
{
	float direction = 1;
	if (this->isLeft())
	{
		direction = -1;
	}

	if (this->getScale().x > 0)
		this->setScaleX(this->getScale().x * (direction)*-1);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(direction*_moveSpeed, move->getVelocity().y));
}

void Fishman::standing()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));
	this->removeStatus(BEING_HIT);
	this->removeStatus(eStatus::JUMPING);
	this->removeStatus(eStatus::FALLING);
}

void Fishman::release()
{
	SAFE_DELETE(_animation);
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();
}

GVector2 Fishman::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}

void Fishman::init()
{
	_moveSpeed = FISHMAN_SPEED;
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
	if (!_isLeft)
	{
		this->setScaleX(-1);
	}
}

void Fishman::Active(bool direct)
{
	if(direct)
		if (_isActive == false)
		{
			this->jump();
			_isActive = true;
			splash();
		}
}

void Fishman::splash()
{
	float x = this->getPositionX();
	float y = this->getPositionY()+50;
	auto splash1 = new Splash(x, y);
	splash1->init();
	auto splash2 = new Splash(x, y);
	splash2->init();
	auto splash3 = new Splash(x, y);
	splash3->init();
	QuadTreeNode::getInstance()->Insert(splash1);
	QuadTreeNode::getInstance()->Insert(splash2);
	QuadTreeNode::getInstance()->Insert(splash3);
	splash1->doSplash(GVector2(20, 100));
	splash2->doSplash(GVector2(-20, 100));
	splash3->doSplash(GVector2(0, 100));
}


bool Fishman::isActive()
{
	return _isActive;
}

bool Fishman::isDead()
{
	return _isDead;
}

bool Fishman::isLeft()
{
	return _isLeft;
}

void Fishman::setStatus(eStatus status)
{
	_status = status;
}