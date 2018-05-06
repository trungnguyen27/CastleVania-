#include "Monkey.h"


Monkey::Monkey(int x, int y, int activeX) : BaseObject(MONKEY)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ENEMY);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ENEMY, "monkey_1"));
	_sprite->setPosition(x, y);
	_sprite->setScale(1.0);
	_animation = new Animation(_sprite, 0.2f);
	_activeXLeft = activeX;
	_activeXRight = x;
	_animation->addFrameRect(eID::ENEMY, "monkey_1", "monkey_2",NULL);

	_effect = SpriteManager::getInstance()->getSprite(eID::EFFECT);
	_effect->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EFFECT, "hit_effect_1"));
	_effectAnimation = new Animation(_effect, 0.15f);
	_effectAnimation->addFrameRect(EFFECT, "hit_effect_1", "hit_effect_2", "hit_effect_3", "hit_effect_4", NULL);

	_hitPoint = 2;
}

void Monkey::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_hitPoint > 0)
		_animation->draw(spriteHandle, viewport);
	else
	{
		_effectAnimation->draw(spriteHandle, viewport);
	}
}

void Monkey::update(float deltatime)
{
	_effect->setPosition(this->getPosition());
	if (_hitPoint > 0)
	{
		_animation->update(deltatime);
		auto move = (Movement*)this->_componentList["Movement"];
		if (_startHit)
		{
			if (_hitStopWatch->isStopWatch(400))
			{
				_startHit = false;
				_hitStopWatch->restart();
			}
		}

		// If not follow the franken, do shaken
		if (!_follow && _shootStopWatch->isTimeLoop(SHOOT_DELAY))
		{
			this->shoot();
		}

		if (_startHit)
		{
			move->setVelocity(GVector2(0, 0));
		}

		if (_follow && followTarget)
		{
			this->setPositionX(followTarget->getPositionX());
			float scale = followTarget->getScale().x / abs(followTarget->getScale().x);
			this->setScale(GVector2(scale, this->getScale().y));
		}
	}
	else
	{
		_effectAnimation->update(deltatime);
		if (_effectStopWatch->isStopWatch(600))
		{
			this->setStatus(DESTROY);
			srand(time(0));
			auto ran = rand() % 10;
			BaseObject* heart = nullptr;
			if (ran < 3)
				heart = new BigHeart(this->getPositionX(), this->getPositionY());
			else
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

void Monkey::shoot()
{
	if (_shootingTarget)
	{
		auto y = this->getPositionY();
		auto vector = GVector2((_shootingTarget->getPositionX() - this->getPositionX()), (_shootingTarget->getPositionY() - this->getPositionY()));
		float magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);
		auto direction = GVector2(vector.x / magnitude, vector.y / magnitude);
		auto fireball = new FireBall(this->getPositionX(), y, _isLeft);
		fireball->init();
		fireball->setDirection(direction);
		QuadTreeNode::getInstance()->Insert(fireball);
	}
	
}

void Monkey::setShootingTarget(BaseObject* target)
{
	_shootingTarget = target;
}

float Monkey::checkCollision(BaseObject* object, float dt)
{
	if (!_activated) return 0.f;

	if (object->getStatus() == eStatus::DESTROY || this->isInStatus(eStatus::DIE))
		return 0.0f;
	if (this == object)
		return 0.0f;

	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;
	if (objectId == WALL || objectId == MONKEY_WALL)
	{
		if (collisionBody->checkCollision(object, direction, dt, false) && !this->isInStatus(MOVING_UP))
		{
			if (direction == TOP)
			{
				float moveX, moveY;
				if (collisionBody->isColliding(object, moveX, moveY, dt))
				{
					collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
					if (objectId != MONKEY_WALL)
						this->jump();
				}
			}
			else  if (direction == LEFT)
			{
				this->turnLeft();
			}
			else if (direction == RIGHT) {
				this->turnRight();
			}

			if (direction == eDirection::TOP)
				if (!(this->getVelocity().y > -200 && this->isInStatus(eStatus::JUMPING)))
					if (!(this->getVelocity().y > -200 && this->isInStatus(eStatus::BEING_HIT)))
					{
						auto gravity = (Gravity*)this->_componentList["Gravity"];
						gravity->setStatus(eGravityStatus::SHALLOWED);

						if (this->isInStatus(eStatus::FALLING) || this->isInStatus(eStatus::BEING_HIT))
						{
							//SoundManager::getInstance()->Play(FALLING_WALL);
						}
				
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

void Monkey::turnLeft()
{
	_isLeft = true;

	this->jump();
}

void Monkey::turnRight()
{
	_isLeft = false;
	this->jump();
}

void Monkey::standing()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));
	this->removeStatus(eStatus::JUMPING);
	this->removeStatus(eStatus::FALLING);
}

void Monkey::jump()
{
	srand(time(NULL));

	int leap = rand() % 150 + 100;
	int angle = rand() % 250 + 200;
	int direction = _isLeft ? -1 : 1;
	//if (this->isInStatus(eStatus::JUMPING) || this->isInStatus(eStatus::FALLING))
	//	return;
	this->setScaleX(this->getScale().x * (-direction));

	this->addStatus(eStatus::JUMPING);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(direction*leap, angle));

	auto g = (Gravity*)this->_componentList["Gravity"];
	g->setStatus(eGravityStatus::FALLING__DOWN);
}

void Monkey::active(bool activate)
{
	_activated = activate;
	if (!_activated )
	{
		this->jump();
	}
}

void Monkey::follow(BaseObject* target)
{
	if (target) {
		followTarget = target;
		_follow = true;
	}
	else _follow = false;
}

void Monkey::release()
{
	SAFE_DELETE(_animation);
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();
}

void Monkey::init()
{
	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;

	auto movement = new Movement(GVector2(0, 0), GVector2(-0, 0), _sprite);
	_componentList["Movement"] = movement;

	_componentList["Gravity"] = new Gravity(GVector2(0, -GRAVITY), movement);
	this->setOrigin(GVector2(0.5f, 0.0f));


	auto g = (Gravity*)this->_componentList["Gravity"];
	g->setStatus(eGravityStatus::SHALLOWED);

	_effectStopWatch = new StopWatch();
	_hitStopWatch = new StopWatch();
	_shootStopWatch = new StopWatch();
	_startHit = false;
}

void Monkey::wasHit(int hitpoint)
{
	if (!_startHit)
	{
		_hitPoint -= hitpoint;
		_hitStopWatch->restart();
		_hitStopWatch->isTimeLoop(400);
		_startHit = true;
	}

	if (_hitPoint <= 0)
	{
		auto move = (Movement*)this->_componentList["Movement"];
		move->setVelocity(GVector2(0, 0));
		_effectStopWatch->isTimeLoop(600);
	}
}

bool Monkey::isDead()
{
	return (_hitPoint <= 0);
}
