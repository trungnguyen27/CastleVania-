#include "Player.h"

Player::Player() : BaseObject(eID::PLAYER)
{
}

int Player::getLifeNumber()
{
	return _info->GetLife();
}

Player::~Player()
{
}

void Player::init()
{
	__hook(&InputController::__eventkeyPressed, _input, &Player::onKeyPressed);
	__hook(&InputController::__eventkeyReleased, _input, &Player::onKeyReleased);

	_sprite = SpriteManager::getInstance()->getSprite(eID::PLAYER);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::PLAYER, "walk_right_01"));
	_sprite->setZIndex(1.0f);

	auto movement = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);

	_componentList["Movement"] = movement;
	_componentList["Gravity"] = new Gravity(GVector2(0, -GRAVITY), movement);

	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;

	_rope = new Rope(0, 0);

	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL]->addFrameRect(eID::PLAYER, "walk_right_01", NULL);

	_animations[eStatus::DIE] = new Animation(_sprite, 2.0f);
	_animations[eStatus::DIE]->addFrameRect(eID::PLAYER, "death", "death", NULL);
	_animations[eStatus::DIE]->animateFromTo(0, 1, false);

	_animations[eStatus::RUNNING] = new Animation(_sprite, 0.08f);
	_animations[eStatus::RUNNING]->addFrameRect(eID::PLAYER, "walk_right_01", "walk_right_02", "walk_right_03", NULL);

	_animations[eStatus::SIT_DOWN] = new Animation(_sprite, 0.1f);
	_animations[eStatus::SIT_DOWN]->addFrameRect(eID::PLAYER, "sit_down", NULL);

	_animations[eStatus::JUMPING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::JUMPING]->addFrameRect(eID::PLAYER, "jump", NULL);

	_animations[eStatus::BEING_HIT] = new Animation(_sprite, 0.1f);
	_animations[eStatus::BEING_HIT]->addFrameRect(eID::PLAYER, "be_hit", NULL);

	_animations[eStatus::FALLING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::FALLING]->addFrameRect(eID::PLAYER, "walk_right_01", NULL);


	_animations[eStatus::MOVING_UP] = new Animation(_sprite, 0.1f);
	_animations[eStatus::MOVING_UP]->addFrameRect(eID::PLAYER, "up", "walk_right_02", NULL);

	_animations[eStatus::MOVING_DOWN] = new Animation(_sprite, 0.1f);
	_animations[eStatus::MOVING_DOWN]->addFrameRect(eID::PLAYER, "down", "walk_right_02", NULL);

	_animations[eStatus::STAND_UP] = new Animation(_sprite, 0.12f);
	_animations[eStatus::STAND_UP]->addFrameRect(eID::PLAYER, "up", NULL);

	_animations[eStatus::STAND_DOWN] = new Animation(_sprite, 0.12f);
	_animations[eStatus::STAND_DOWN]->addFrameRect(eID::PLAYER, "down",NULL);


	_animations[eStatus::ATTACKING] = new Animation(_sprite, 0.2f);
	_animations[eStatus::ATTACKING]->addFrameRect(eID::PLAYER, "stand_and_hit_01", "stand_and_hit_02", "stand_and_hit_03", "stand_and_hit_03", NULL);

	_animations[STAND_UP | ATTACKING] = new Animation(_sprite, 0.2f);
	_animations[STAND_UP | ATTACKING]->addFrameRect(eID::PLAYER, "up_and_hit_01", "up_and_hit_02", "up_and_hit_03", "up_and_hit_03", NULL);

	_animations[STAND_DOWN | ATTACKING] = new Animation(_sprite, 0.2f);
	_animations[STAND_DOWN | ATTACKING]->addFrameRect(eID::PLAYER, "down_and_hit_01", "down_and_hit_02", "down_and_hit_03", "down_and_hit_03", NULL);

	_animations[SIT_DOWN | ATTACKING] = new Animation(_sprite, 0.2f);
	_animations[SIT_DOWN | ATTACKING]->addFrameRect(eID::PLAYER, "jump_and_hit_01", "jump_and_hit_02", "jump_and_hit_03", "jump_and_hit_03", NULL);

	_animations[JUMPING | ATTACKING] = new Animation(_sprite, 0.2f);
	_animations[JUMPING | ATTACKING]->addFrameRect(eID::PLAYER, "jump_and_hit_01", "jump_and_hit_02", "jump_and_hit_03", "jump_and_hit_03", NULL);

	_animations[FALLING | ATTACKING] = new Animation(_sprite, 0.2f);
	_animations[FALLING | ATTACKING]->addFrameRect(eID::PLAYER, "jump_and_hit_01", "jump_and_hit_02", "jump_and_hit_03", "jump_and_hit_03", NULL);

	this->setOrigin(GVector2(0.5f, 0.0f));
	this->setStatus(eStatus::NORMAL);

	// create stopWatch
	_stairStopWatch = new StopWatch();
	_attackStopWatch = new StopWatch();
	_weaponStopWatch = new StopWatch();
	this->_isRevive = false;
	this->_isBack = false;
	this->_currentAnimateIndex = NORMAL;
	this->_cross = false;
	this->_endLevel = false;
	_info = new Info();
	_info->init();
	_info->SetHeart(50);
	_info->SetLife(3);
	_info->SetScore(0);
	_info->SetMaxWeapon(1);
	_info->SetPlayerHitPoint(16);
	_info->SetEnemyHitPoint(16);
	_info->ActiveTime();
	_info->SetTime(400);
	this->resetValues();
}

void Player::updateAttackStatus(float dt)
{
	if (_isAttacking)
	{
		if (_attackStopWatch->isStopWatch(ATTACK_TIME))
		{
			_isAttacking = false;
			this->removeStatus(ATTACKING);
			if (_isRope)
				this->_rope->restart();
		}
		if (_isAttacking && !_isRope && _weaponStopWatch->isTimeLoop(ATTACK_TIME / 2) && _listWeapon.size() < _info->GetMaxWeapon())
		{
			Weapon* weapon = nullptr;
			switch (_info->GetCurrentWeapon())
			{
			case DAGGER:
			{
				if (_info->GetHeart() > 0 && !SoundManager::getInstance()->IsPlaying(DAGGER_SOUND))
				{
					if (this->getScale().x > 0)
						weapon = new DaggerWeapon(this->getPositionX() + 16, this->getPositionY() + 40, true);
					else
						weapon = new DaggerWeapon(this->getPositionX() - 16, this->getPositionY() + 40, false);
					_info->SetHeart(_info->GetHeart() - 1);

					SoundManager::getInstance()->Play(DAGGER_SOUND);
				}
				break;
			}
			case BOOMERANG:
			{
				if (_info->GetHeart() > 0)
				{
					if (this->getScale().x > 0)
						weapon = new BoomerangWeapon(this->getPositionX() + 16, this->getPositionY() + 30, true);
					else
						weapon = new BoomerangWeapon(this->getPositionX() - 16, this->getPositionY() + 30, false);
					_info->SetHeart(_info->GetHeart() - 1);
				}
				break;
			}
			case HOLYWATER:
			{
				if (_info->GetHeart() > 0)
				{
					if (this->getScale().x > 0)
						weapon = new HolyWaterWeapon(this->getPositionX() + 10, this->getPositionY() + 30, true);
					else
						weapon = new HolyWaterWeapon(this->getPositionX() - 10, this->getPositionY() + 30, false);
					_info->SetHeart(_info->GetHeart() - 1);
				}
				break;
			}
			case AXE:
			{
				if (_info->GetHeart() > 0)
				{
					if (this->getScale().x > 0)
						weapon = new AxeWeapon(this->getPositionX() + 16, this->getPositionY() + 40, true);
					else
						weapon = new AxeWeapon(this->getPositionX() - 16, this->getPositionY() + 40, false);
					_info->SetHeart(_info->GetHeart() - 1);
				}
				break;
			}
			default:
				break;
			}
			if (weapon != nullptr)
			{
				weapon->init();
				_listWeapon.push_back(weapon);
			}
		}
	}

	if (!_listWeapon.empty())
	{
		auto viewport = SceneManager::getInstance()->getCurrentScene()->getViewport();
		RECT viewportBounding = viewport->getBounding();
		auto i = 0;
		while (i < _listWeapon.size())
		{
			auto obj = _listWeapon[i];
			if (obj->getId() == BOOMERANG)
			{
				if (obj->getPositionX() + 16 > viewportBounding.right || obj->getPositionX() - 16 < viewportBounding.left)
				{
					if (!((BoomerangWeapon*)obj)->IsBoomerangComeBack())
					{
						((BoomerangWeapon*)obj)->ComeBack();
						i++;
						continue;
					}
				}

				if (((BoomerangWeapon*)obj)->IsBoomerangComeBack())
				{
					auto weaponCollision = _listWeapon[i]->getCollisionBody();
					eDirection direction;
					if (weaponCollision->checkCollision(this, direction, dt, false))
					{
						_listWeapon.erase(_listWeapon.begin() + i);
						obj->release();
						delete obj;
						continue;
					}
				}
			}
			if (!isIntersectedInGame(viewportBounding, _listWeapon[i]->getBounding()) && _listWeapon[i]->getBounding().top < viewportBounding.top)
			{
				_listWeapon.erase(_listWeapon.begin() + i);
				obj->release();
				delete obj;
			}
			else
				i++;
		}
	}

	for (auto weapon : _listWeapon)
	{
		switch (weapon->getId())
		{
		case DAGGER:
			if (!SoundManager::getInstance()->IsPlaying(DAGGER_SOUND))
				SoundManager::getInstance()->Play(DAGGER_SOUND);
			break;
		default:
			break;
		}
	}
}


void Player::update(float deltatime)
{
	if (_endLevel)
	{
		if (SoundManager::getInstance()->IsPlaying(WIN_LEVEL) == false)
		{
			if (SoundManager::getInstance()->IsPlaying(PLAY_SCENE) == false)
				SoundManager::getInstance()->Play(PLAY_SCENE);
			_info->PauseTime();
			if (_info->GetTime() > 0)
			{
				_info->AddScore(10);
				_info->SetTime(_info->GetTime() - 1);
			}
			else if (_info->GetHeart() > 0)
			{
				_info->AddScore(100);
				_info->SetHeart(_info->GetHeart() - 1);
			}
			else
			{
				_isChangedStage = true;
				_info->ActiveTime();
				_info->SetTime(300);
			}
		}
	}
	else
	{
		auto time = _info->GetTime();

		if (time < 4 && time > 0)
			if (!SoundManager::getInstance()->IsPlaying(TIME_OUT))
				SoundManager::getInstance()->Play(TIME_OUT);

		if (_info->GetPlayerHitPoint() == 0 || time == 0)
		{
			_info->PauseTime();
			_info->SetTime(time);
			this->setStatus(DIE);
			if (!SoundManager::getInstance()->IsPlaying(DIE_SOUND))
				SoundManager::getInstance()->Play(DIE_SOUND);
			auto move = (Movement*)this->_componentList["Movement"];
			move->setVelocity(GVector2(0, this->getVelocity().y));
			_protectTime = 0;
		}
	}
	if (_isPlayingMovie)
	{
		if (_movieStartMove)
		{
			if (_mapDirect == RIGHT)
			{
				if (this->getPosition().x > _endMoviePosX)
					this->setStatus(eStatus::MOVING_LEFT);
				else
					this->setStatus(eStatus::NORMAL);
			}
			else
			{
				if (this->getPosition().x < _endMoviePosX)
					this->setStatus(eStatus::MOVING_RIGHT);
				else
					this->setStatus(eStatus::NORMAL);
			}
		}
	}

	if (_protectTime > 0)
	{
		_protectTime -= deltatime;
	}

	_info->update(deltatime);

	this->checkPosition();

	this->updateAttackStatus(deltatime);

	this->updateStatus(deltatime);

	this->updateCurrentAnimateIndex();

	_animations[_currentAnimateIndex]->update(deltatime);

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
	for (auto weapon : _listWeapon)
	{
		weapon->update(deltatime);
	}
}

void Player::updateInput(float dt)
{
}

GVector2 Player::getPosition()
{
	return _sprite->getPosition();
}


void Player::updateStatus(float dt)
{
	if (this->isInStatus(eStatus::DIE))
	{
		if (_info->GetLife() < 0)
		{
			return;
		}
		if (!_animations[eStatus::DIE]->isAnimate())
		{
			this->revive();
		}
		return;
	}
	if (_isAttacking)
	{
		if (_isRope)
		{
			_rope->setScaleX(this->_sprite->getScale().x);
			_rope->update(dt);
			if ((this->getStatus() & eStatus::SIT_DOWN) == eStatus::SIT_DOWN || (this->getStatus() & eStatus::JUMPING) == eStatus::JUMPING)
				_rope->updateRopePos(this->getPosition() + GVector2(0, -16));
			else
				_rope->updateRopePos(this->getPosition());
		}
	}

	if ((this->getStatus() & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT)
	{
		this->moveLeft();
	}
	else if ((this->getStatus() & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT)
	{
		this->moveRight();
	}
	//else if ((this->getStatus() & eStatus::SIT_DOWN) == eStatus::SIT_DOWN)
	//{
	//	
	//}
	else if ((this->getStatus() & eStatus::FALLING) == eStatus::FALLING)
	{
		this->falling();
	}
	else if ((this->getStatus() & eStatus::MOVING_UP) == eStatus::MOVING_UP)
	{
		moveUp();
	}
	else if ((this->getStatus() & eStatus::MOVING_DOWN) == eStatus::MOVING_DOWN)
	{
		moveDown();
	}
	else if (this->getStatus() == eStatus::STAND_DOWN)
	{
		if (_input->isKeyDown(DIK_J))
			this->setStatus(MOVING_DOWN);
	}
	else if (this->getStatus() == eStatus::STAND_UP)
	{
		if (_input->isKeyDown(DIK_U))
			this->setStatus(MOVING_UP);
	} else if ((this->getStatus() & eStatus::JUMPING) != eStatus::JUMPING && !this->isInStatus(BEING_HIT))
	{
		if ((this->getStatus() & eStatus::SIT_DOWN) == eStatus::SIT_DOWN) {
			this->sitDown();
		}
		this->standing();
		if (preWall != nullptr)
			if (preWall->getId() == FLOATING_WALL)
			{
				auto move = (Movement*)this->_componentList["Movement"];
				move->setVelocity(((FloatingWall*)preWall)->getVelocity());
			}
	}
}

void Player::updateCurrentAnimateIndex()
{
	_currentAnimateIndex = this->getStatus();

	if ((_currentAnimateIndex & eStatus::FALLING) == eStatus::FALLING)
	{
		_currentAnimateIndex = eStatus::FALLING;
	}

	if ((_currentAnimateIndex & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT || ((_currentAnimateIndex & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT))
	{
		_currentAnimateIndex = (eStatus)(_currentAnimateIndex & ~(eStatus::MOVING_LEFT | MOVING_RIGHT));
		if ((_currentAnimateIndex & eStatus::MOVING_UP) == eStatus::MOVING_UP)
		{
			_currentAnimateIndex = eStatus::MOVING_UP;
		}
		else if ((_currentAnimateIndex & eStatus::MOVING_DOWN) == eStatus::MOVING_DOWN)
		{
			_currentAnimateIndex = eStatus::MOVING_DOWN;
		}
		else
			_currentAnimateIndex = (eStatus)(_currentAnimateIndex | eStatus::RUNNING);
	}


	if ((_currentAnimateIndex & eStatus::JUMPING) == eStatus::JUMPING)
	{
		if ((_currentAnimateIndex & eStatus::ATTACKING) != eStatus::ATTACKING)
			_currentAnimateIndex = eStatus::JUMPING;
	}


	if (this->isInStatus(eStatus::DIE))
	{
		_currentAnimateIndex = eStatus::DIE;
	}

	if ((_currentAnimateIndex & eStatus::BEING_HIT) == eStatus::BEING_HIT)
		_currentAnimateIndex = eStatus::BEING_HIT;
}

void Player::resetValues()
{
	_info->ActiveTime();
	_listWeapon.clear();
	preWall = nullptr;
	_endMoviePosX = -1;
	_isPlayingMovie = false;
	_movieStartMove = false;
	_isAttacking = false;
	_stair = nullptr;
	_stairEnd = nullptr;
	_holdingKey = false;

	_movingSpeed = MOVE_SPEED;

	_info->SetEnemyHitPoint(16);
	if (_endLevel)
	{
		this->setScale(SCALE_FACTOR);
		_protectTime = PROTECT_TIME;
		_info->SetPlayerHitPoint(16);
		_endLevel = false;
	}

	if (_isRevive)
	{
		_protectTime = PROTECT_TIME;
		_info->SetTime(300);
		_rope->resetRope();
		_info->SetPlayerHitPoint(16);
		this->setStatus(NORMAL);
		auto gravity = (Gravity*)this->_componentList["Gravity"];
		gravity->setStatus(eGravityStatus::FALLING__DOWN);
		this->setScale(SCALE_FACTOR);
		this->setPosition(this->_revivePos);
	}
	else if (_isBack)
	{
		this->setPosition(_backPos);
		this->setStatus(_backStatus);
	}

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));

	for (auto animate : _animations)
	{
		animate.second->setColorFlash(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	_isRevive = false;
	_isBack = false;
}

void Player::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_protectTime > 0)
	{
		_animations[_currentAnimateIndex]->enableFlashes(true);
	}
	else
	{
		_animations[_currentAnimateIndex]->enableFlashes(false);
	}

	_animations[_currentAnimateIndex]->draw(spriteHandle, viewport);

	if (_isAttacking)
	{
		if (_isRope)
			_rope->draw(spriteHandle, viewport);
	}

	_info->draw(spriteHandle, viewport);

	for (auto weapon : _listWeapon)
	{
		weapon->draw(spriteHandle, viewport);
	}
}

void Player::release()
{
	for (auto it = _animations.begin(); it != _animations.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_animations.clear();

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();

	SAFE_DELETE(_sprite);
	SAFE_DELETE(_stairStopWatch);
	SAFE_DELETE(_info);
	this->unhookinputevent();
}

void Player::onKeyPressed(KeyEventArg* key_event)
{
	if (_isPlayingMovie)
		return;
	if (this->isInStatus(eStatus::DIE))
		return;
	if (_isAttacking)
		return;
	switch (key_event->_key)
	{
		case DIK_H:
			{
				if (this->isInStatus(SIT_DOWN))
				{
					if (this->getScale().x > 0)
						this->setScaleX(this->getScale().x * (-1));
					break;
				}
				if (this->isInStatus(STAND_UP) || this->isInStatus(MOVING_UP) || this->isInStatus(STAND_DOWN) || this->isInStatus(MOVING_DOWN))
				{
					if (!_directStair)
					{
						moveUp();
						_holdingKey = true;
					}
					else
					{
						moveDown();
						_holdingKey = true;
					}
				}
				else if (!this->isInStatus(eStatus::JUMPING))
				{
					this->removeStatus(eStatus::MOVING_RIGHT);
					this->addStatus(eStatus::MOVING_LEFT);
				}

				break;
			}
		case DIK_K:
			{
				if (this->isInStatus(SIT_DOWN))
				{
					if (this->getScale().x < 0)
						this->setScaleX(this->getScale().x * (-1));
					break;
				}
				if (this->isInStatus(STAND_UP) || this->isInStatus(MOVING_UP) || this->isInStatus(STAND_DOWN) || this->isInStatus(MOVING_DOWN))
				{
					if (_directStair)
					{
						moveUp();
						_holdingKey = true;
					}
					else
					{
						moveDown();
						_holdingKey = true;
					}
				}
				else if (!this->isInStatus(eStatus::JUMPING))
				{
					this->removeStatus(eStatus::MOVING_LEFT);
					this->addStatus(eStatus::MOVING_RIGHT);
				}
				break;
			}
		case DIK_J:
			{
				if (this->isInStatus(STAND_UP) || this->isInStatus(STAND_DOWN) || this->isInStatus(MOVING_DOWN) || this->isInStatus(MOVING_UP))
				{
					moveDown();
					_holdingKey = true;
				}
				else if (!this->isInStatus(eStatus::JUMPING))
				{
					this->addStatus(eStatus::SIT_DOWN);
					this->removeStatus(eStatus::MOVING_LEFT);
					this->removeStatus(eStatus::MOVING_RIGHT);
				}
				break;
			}

		case DIK_X:
			{
				if (this->isInStatus(STAND_UP) || this->isInStatus(STAND_DOWN) || this->isInStatus(MOVING_UP) || this->isInStatus(MOVING_DOWN))
					break;
				if (!this->isInStatus(eStatus::SIT_DOWN) || this->isInStatus(eStatus::MOVING_LEFT) || this->isInStatus(eStatus::MOVING_RIGHT))
					this->jump();
				break;
			}
		case DIK_U:
			{
				if ((this->getStatus() & eStatus::JUMPING) != eStatus::JUMPING)
					if (_stair != nullptr)
					{
						moveUp();
						_holdingKey = true;
					}
				break;
			}
		case DIK_Z:
			{
				if (this->isInStatus(MOVING_DOWN) || this->isInStatus(MOVING_UP))
				{
					break;
				}
				this->hit();
				break;
			}
		case DIK_Q:
		{
			_rope->upgradeRope();
			break;
		}
		case DIK_1:
		{
			_info->SetWeapon(DAGGER);
			break;
		}
		case DIK_2:
		{
			_info->SetWeapon(HOLYWATER);
			break;
		}
		case DIK_3:
		{
			_info->SetWeapon(BOOMERANG);
			break;
		}
		case DIK_4:
		{
			_info->SetWeapon(AXE);
			break;
		}
		case DIK_W:
			{
				_info->SetMaxWeapon(_info->GetMaxWeapon() + 1);
				break;
			}
		default:
			break;
	}
}

void Player::onKeyReleased(KeyEventArg* key_event)
{
	if (this->isInStatus(eStatus::DIE))
		return;

	switch (key_event->_key)
	{
		case DIK_K:
			{
				if (this->isInStatus(STAND_UP) || this->isInStatus(STAND_DOWN) || this->isInStatus(MOVING_UP) || this->isInStatus(MOVING_DOWN))
					_holdingKey = false;

				this->removeStatus(eStatus::MOVING_RIGHT);
				break;
			}
		case DIK_H:
			{
				if (this->isInStatus(STAND_UP) || this->isInStatus(STAND_DOWN) || this->isInStatus(MOVING_UP) || this->isInStatus(MOVING_DOWN))
					_holdingKey = false;
				this->removeStatus(eStatus::MOVING_LEFT);
				break;
			}
		case DIK_J:
			{
				this->removeStatus(eStatus::SIT_DOWN);
				if (this->isInStatus(MOVING_DOWN))
				{
					_holdingKey = false;
				}
				break;
			}
		case DIK_U:
			{
				_holdingKey = false;
				break;
			}
		case DIK_Z:
			{
				break;
			}
		default:
			break;
	}
}

bool Player::weaponCheckCollision(BaseObject* object, eDirection& direction, float dt, bool updatePosition)
{
	if (!_listWeapon.empty())
	{
		auto i = 0;
		while (i < _listWeapon.size())
		{
			auto weaponCollision = _listWeapon[i]->getCollisionBody();
			if (weaponCollision->checkCollision(object, direction, dt, false))
			{
				if (_listWeapon[i]->getId() == DAGGER)
				{
					auto obj = _listWeapon[i];
					_listWeapon.erase(_listWeapon.begin() + i);
					obj->release();
					delete obj;
				}
				if (!SoundManager::getInstance()->IsPlaying(COLISION_SOUND))
					SoundManager::getInstance()->Play(COLISION_SOUND);
				return true;
			}
			i++;
		}
	}
	return false;
}

float Player::checkCollision(BaseObject* object, float dt)
{
	if (object->getStatus() == eStatus::DESTROY || this->isInStatus(eStatus::DIE))
		return 0.0f;
	if (this == object)
		return 0.0f;

	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	auto ropeCollisionBody = _rope->getCollisionBody();

	eID objectId = object->getId();
	eDirection direction;

	if (objectId == WALL)
	{
		if (collisionBody->checkCollision(object, direction, dt, false) && !this->isInStatus(MOVING_UP))
		{
			if (direction != BOTTOM)
			{
				float moveX, moveY;
				if (collisionBody->isColliding(object, moveX, moveY, dt))
				{
					collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
				}
			}

			if (_protectTime <= 0)
				this->isInStatus(eStatus::JUMPING);
			else
				this->isInStatus(eStatus::JUMPING);

			if (direction == eDirection::TOP)
				if (!(this->getVelocity().y > -200 && this->isInStatus(eStatus::JUMPING)))
					if (!(this->getVelocity().y > -200 && this->isInStatus(eStatus::BEING_HIT)))
					{
						auto gravity = (Gravity*)this->_componentList["Gravity"];
						gravity->setStatus(eGravityStatus::SHALLOWED);

						if (this->isInStatus(eStatus::FALLING) || this->isInStatus(eStatus::BEING_HIT))
						{
							SoundManager::getInstance()->Play(FALLING_WALL);
						}
						if (this->isInStatus(eStatus::BEING_HIT))
						{
							_info->SetPlayerHitPoint(_info->GetPlayerHitPoint() - 2);
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
	else if (objectId == CANDLE)
	{
		if (ropeCollisionBody->checkCollision(object, direction, dt, false))
		{
			((Candle*)object)->wasHit();
			if (!SoundManager::getInstance()->IsPlaying(COLISION_SOUND))
				SoundManager::getInstance()->Play(COLISION_SOUND);
		}
	}
	else if (objectId == HEART) {
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			_info->SetHeart(_info->GetHeart() + 1);
			object->setStatus(DESTROY);
			if (!SoundManager::getInstance()->IsPlaying(GET_HEART))
				SoundManager::getInstance()->Play(GET_HEART);
		}
	}
	else if (objectId == BIGHEART) {
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			_info->SetHeart(_info->GetHeart() + 5);
			object->setStatus(DESTROY);
			if (!SoundManager::getInstance()->IsPlaying(GET_HEART))
				SoundManager::getInstance()->Play(GET_HEART);
		}
	}
	else if (objectId == YELLOWBAG) {
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			object->setStatus(DESTROY);
			if (!SoundManager::getInstance()->IsPlaying(GET_ITEM))
				SoundManager::getInstance()->Play(GET_ITEM);
		}
	}
	else if (objectId == REDBAG) {
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			object->setStatus(DESTROY);
			if (!SoundManager::getInstance()->IsPlaying(GET_ITEM))
				SoundManager::getInstance()->Play(GET_ITEM);
		}
	}
	else if (objectId == STOPTIME) {
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			object->setStatus(DESTROY);
			if (!SoundManager::getInstance()->IsPlaying(GET_ITEM))
				SoundManager::getInstance()->Play(GET_ITEM);
		}
	}
	else if (objectId == POTION)
	{
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			_protectTime = 5000;
			object->setStatus(DESTROY);
			if (!SoundManager::getInstance()->IsPlaying(GET_POTION))
				SoundManager::getInstance()->Play(GET_POTION);
		}
	}
	else if (objectId == HOLYWATER)
	{
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			_protectTime = 5000;
			object->setStatus(DESTROY);
		}
	}
	else if (objectId == DAGGER)
	{
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			if (_info->GetCurrentWeapon() != DAGGER)
				_info->SetMaxWeapon(1);
			_info->SetWeapon(DAGGER);
			object->setStatus(DESTROY);
			if (!SoundManager::getInstance()->IsPlaying(GET_ITEM))
				SoundManager::getInstance()->Play(GET_ITEM);
		}
	}
	else if (objectId == FIREBALL)
	{
		if (!((FireBall*)object)->isDead() && _protectTime <= 0)
		{
			if (collisionBody->checkCollision(object, direction, dt, false))
			{
				float moveX, moveY;
				if (collisionBody->isColliding(object, moveX, moveY, dt))
				{
					collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
				}
				behit(direction);
			}
			if (ropeCollisionBody->checkCollision(object, direction, dt, false))
			{
				((FireBall*)object)->wasHit();
				if (!SoundManager::getInstance()->IsPlaying(COLISION_SOUND))
					SoundManager::getInstance()->Play(COLISION_SOUND);
			}
			if (this->weaponCheckCollision(object, direction, dt, false))
			{
				((FireBall*)object)->wasHit();
			}
		}
	}
	else if (objectId == BAT)
	{
		if (!((Bat*)object)->isActive())
		{
			auto objPos = object->getPosition();
			auto pos = this->getPosition();
			if (getdistance(objPos, pos) < 500 && abs(pos.x - objPos.x) < 500)
			{
				((Bat*)object)->Active(pos.x > objPos.x);
			}
		}

		if (!((Bat*)object)->isDead() && _protectTime <= 0)
		{
			if (collisionBody->checkCollision(object, direction, dt, false))
			{
				float moveX, moveY;
				if (collisionBody->isColliding(object, moveX, moveY, dt))
				{
					collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
				}
				behit(direction);
			}
			if (ropeCollisionBody->checkCollision(object, direction, dt, false))
			{
				((Bat*)object)->wasHit();
				if (!SoundManager::getInstance()->IsPlaying(COLISION_SOUND))
					SoundManager::getInstance()->Play(COLISION_SOUND);
			}
			if (this->weaponCheckCollision(object, direction, dt, false))
			{
				((Bat*)object)->wasHit();
			}
			if (((Bat*)object)->isDead())
				_info->AddScore(200);
		}
	}
	else if (objectId == FISHMAN) {
		Fishman* fishman = (Fishman*)object;
		if (!fishman->isActive())
		{
			auto objPos = object->getPosition();
			auto pos = this->getPosition();
			bool active = false;
			if (fishman->isLeft())
				active = objPos.x - pos.x < 100;
			else active = pos.x - objPos.x > 100;
			fishman->Active(active);
		}
		if (!(fishman->isDead() && _protectTime <= 0))
		{
			if (collisionBody->checkCollision(object, direction, dt, false))
			{
				float moveX, moveY;
				if (collisionBody->isColliding(object, moveX, moveY, dt))
				{
					collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
				}
				behit(direction);
			}
			if (ropeCollisionBody->checkCollision(object, direction, dt, false))
			{
				object->setStatus(DESTROY);
			}
			if (this->weaponCheckCollision(object, direction, dt, false))
			{
				object->setStatus(DESTROY);
			}
		}
	}
	else if (objectId == MONKEY)
	{
		if (ropeCollisionBody->checkCollision(object, direction, dt, false))
		{
			//object->setStatus(DESTROY);
		}
		if (this->weaponCheckCollision(object, direction, dt, false))
		{
			//object->setStatus(DESTROY);
		}
	}
	else if (objectId == WATER) {
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			this->revive();
		}
	}
	else if (objectId == FLOATING_WALL)
	{
		if (collisionBody->checkCollision(object, direction, dt, false) && !this->isInStatus(MOVING_UP))
		{
			if (direction == eDirection::TOP)
			{
				if (!(this->getVelocity().y > -200 && this->isInStatus(eStatus::JUMPING)))
					if (!(this->getVelocity().y > -200 && this->isInStatus(eStatus::BEING_HIT)))
					{
						auto gravity = (Gravity*)this->_componentList["Gravity"];
						gravity->setStatus(eGravityStatus::SHALLOWED);
						if (this->isInStatus(eStatus::BEING_HIT))
						{
							_info->SetPlayerHitPoint(_info->GetPlayerHitPoint() - 2);
						}
						this->standing();
						preWall = object;
						float moveX, moveY;
						if (collisionBody->isColliding(object, moveX, moveY, dt))
						{
							collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY - 4));
						}
					}
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
	else if (objectId == FRANKENSTEIN)
	{
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
		}
		auto objPos = object->getPosition();
		auto pos = this->getPosition();
		Frankenstein* frankenstein = (Frankenstein*)object;

		float deltaX = abs(pos.x - objPos.x);
		if (!frankenstein->isActive())
		{
			if (getdistance(objPos, pos) < 400 && deltaX < 400)
			{
				frankenstein->Active(true);
				frankenstein->follow(this);
				auto checkPoint = frankenstein->getCheckpoint();
				StageManager::getInstance()->getCurrentTileMap()->setCheckpoint(checkPoint);
				auto wall = new Wall(checkPoint + 20, 0, 16, 512);
				QuadTreeNode::getInstance()->Insert(wall);
				SoundManager::getInstance()->Stop(PLAY_SCENE);
				if (!SoundManager::getInstance()->IsPlaying(BOSS_SOUND))
					SoundManager::getInstance()->PlayLoop(BOSS_SOUND);
			}
		}
		else {
			if (getdistance(objPos, pos) > 400 && deltaX > 400)
			{
				frankenstein->Active(false);
			}
			if (deltaX < 100)
			{
				frankenstein->releaseMonkey();
			}

			if (ropeCollisionBody->checkCollision(object, direction, dt, false))
			{
				frankenstein->beHit();
				_info->SetEnemyHitPoint(_info->GetEnemyHitPoint() - 2);
				if (!SoundManager::getInstance()->IsPlaying(COLISION_SOUND))
					SoundManager::getInstance()->Play(COLISION_SOUND);
			}
			if (this->weaponCheckCollision(object, direction, dt, false))
			{
				frankenstein->beHit();
				if (!SoundManager::getInstance()->IsPlaying(COLISION_SOUND))
					SoundManager::getInstance()->Play(COLISION_SOUND);
			}
		}

		auto frankensteinHitPoints = frankenstein->getHitPoint();
		_info->SetEnemyHitPoint(frankensteinHitPoints);
		if (!frankenstein->isDead() && (frankensteinHitPoints == 0))
		{
			_info->AddScore(3000);
		}
	}
	else if (objectId == DINOSAUR)
	{
		Dinosaur* dinosaur = (Dinosaur*)object;
		if (!dinosaur->isActive())
		{
			auto objPos = object->getPosition();
			auto pos = this->getPosition();
			bool active = objPos.x - pos.x < 100;	
			dinosaur->Active(active);
			dinosaur->setShootingTarget(this);
		}
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			float moveX, moveY;
			if (collisionBody->isColliding(object, moveX, moveY, dt))
			{
				collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
			}
			behit(direction);
		}
		if (ropeCollisionBody->checkCollision(object, direction, dt, false))
		{
			dinosaur->wasHit(1);
			if (!SoundManager::getInstance()->IsPlaying(HIT_DINOSAUR))
				SoundManager::getInstance()->Play(HIT_DINOSAUR);
		}
		if (this->weaponCheckCollision(object, direction, dt, false))
		{
			((Dinosaur*)object)->wasHit(2);
			if (!SoundManager::getInstance()->IsPlaying(HIT_DINOSAUR))
				SoundManager::getInstance()->Play(HIT_DINOSAUR);
		}
	}
	else if (objectId == DINOSAUR_BODY)
	{
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			float moveX, moveY;
			if (collisionBody->isColliding(object, moveX, moveY, dt))
			{
				collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
			}
			behit(direction);
		}
	}
	else if (objectId == END)
	{
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			this->_currentStage = ((End*)object)->getNextStage();
			this->_isChangedStage = true;
			this->_backPos = ((End*)object)->getPosition();
			this->_backStatus = ((End*)object)->getStatus();
		}
	}
	else if (objectId == REVIVE)
	{
		if (collisionBody->checkCollision(object, direction, dt, false))
		{
			_revivePos = object->getPosition();
			_reviveStage = this->_currentStage;
			switch (_currentStage)
			{
				case MAP_STAGE_10:
					_info->SetStage(10);
					break;
				case MAP_STAGE_12:
					_info->SetStage(12);
					break;
				default:
					//_info->SetStage(4);
					break;
			}
		}
	};
	return 1.0f;
}

void Player::checkPosition()
{
	if (this->isInStatus(eStatus::DIE))
		return;

	auto viewport = SceneManager::getInstance()->getCurrentScene()->getViewport();
	GVector2 viewport_position = viewport->getPositionWorld();

	if (this->getPositionY() < viewport_position.y - WINDOW_HEIGHT)
	{
		if (_status != eStatus::DIE)
			_status = eStatus::DIE;
		if (_protectTime > 0)
			_protectTime = 0;
		this->die();
	}
}

void Player::standing()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));
	this->removeStatus(BEING_HIT);
	this->removeStatus(eStatus::JUMPING);
	this->removeStatus(eStatus::FALLING);
}

void Player::moveLeft()
{
	if (this->getScale().x > 0)
		this->setScaleX(this->getScale().x * (-1));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(-_movingSpeed, move->getVelocity().y));
}

void Player::moveRight()
{
	if (this->getScale().x < 0)
		this->setScaleX(this->getScale().x * (-1));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(_movingSpeed, move->getVelocity().y));
}

void Player::moveDown()
{
	this->removeStatus(MOVING_UP);
	this->removeStatus(STAND_DOWN);
	this->removeStatus(eStatus::STAND_UP);
	this->addStatus(eStatus::MOVING_DOWN);

	auto move = (Movement*)this->_componentList["Movement"];

	if (!_directStair)
	{
		if (this->getScale().x < 0)
			this->setScaleX(this->getScale().x * (-1));
		move->setVelocity(GVector2(_movingSpeed, -_movingSpeed));
	}
	else
	{
		if (this->getScale().x > 0)
			this->setScaleX(this->getScale().x * (-1));
		move->setVelocity(GVector2(-_movingSpeed, -_movingSpeed));
	}

	int y = this->getPositionY() - _stair->getBounding().bottom;

	y = (y + 9) / 16 * 16;

	if (y == 0)
	{
		if (_directStair)
			this->setPosition(_stair->getBounding().left, _stair->getBounding().bottom);
		else
			this->setPosition(_stair->getBounding().right, _stair->getBounding().bottom);
		this->setStatus(NORMAL);
	}


	if (_stairStopWatch->isTimeLoop(120))
	{
		if (!_holdingKey)
		{
			move->setVelocity(GVector2(0, 0));

			if (_directStair)
			{
				this->setPositionX(y + _stair->getBounding().left);
				this->setPositionY(y + _stair->getBounding().bottom);
			}
			else
			{
				this->setPositionX(-y + _stair->getBounding().right);
				this->setPositionY(y + _stair->getBounding().bottom);
			}

			this->setStatus(STAND_DOWN);
		}

		_stairStopWatch->restart();
	}
}

void Player::moveUp()
{
	if (this->getStatus() == NORMAL || this->isInStatus(MOVING_LEFT) || this->isInStatus(MOVING_RIGHT) || this->isInStatus(RUNNING))
	{
		if (_directStair)
		{
			if (this->getPositionX() > _stair->getBounding().left + 32 || this->getPositionY() + 8 < _stair->getBounding().bottom)
				return;
			this->setPositionX(_stair->getBounding().left);
		}
		else
		{
			if (this->getPositionX() < _stair->getBounding().right - 32 || this->getPositionY() + 8 < _stair->getBounding().bottom)
				return;
			this->setPositionX(_stair->getBounding().right);
		}
	}

	this->setStatus(eStatus::MOVING_UP);

	auto move = (Movement*)this->_componentList["Movement"];


	if (_stair == nullptr)
	{
		this->setStatus(NORMAL);
		_stairStopWatch->restart();
		return;
	}

	if (_directStair)
	{
		if (this->getScale().x < 0)
			this->setScaleX(this->getScale().x * (-1));
		move->setVelocity(GVector2(_movingSpeed, _movingSpeed));
	}
	else
	{
		if (this->getScale().x > 0)
			this->setScaleX(this->getScale().x * (-1));
		move->setVelocity(GVector2(-_movingSpeed, _movingSpeed));
	}

	if (_stairStopWatch->isTimeLoop(120))
	{
		if (!_holdingKey)
		{
			move->setVelocity(GVector2(0, 0));

			if (_directStair)
			{
				int x = this->getPositionX() - _stair->getBounding().left;
				int y = this->getPositionY() - _stair->getBounding().bottom;

				x = x / 16 * 16;

				if (x == 0)
				{
					x = 1;
				}

				this->setPositionX(x + _stair->getBounding().left);
				this->setPositionY(x + _stair->getBounding().bottom);
			}
			else
			{
				int y = this->getPositionY() - _stair->getBounding().bottom;
				y = y / 16 * 16;

				if (y == 0)
				{
					y = 1;
				}
				this->setPositionX(-y + _stair->getBounding().right);
				this->setPositionY(y + _stair->getBounding().bottom);
			}
			this->setStatus(STAND_UP);
		}

		_stairStopWatch->restart();
	}
}

void Player::jump()
{
	if (this->isInStatus(eStatus::JUMPING) || this->isInStatus(eStatus::FALLING))
		return;

	this->addStatus(eStatus::JUMPING);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(move->getVelocity().x, JUMP_VEL));

	auto g = (Gravity*)this->_componentList["Gravity"];
	g->setStatus(eGravityStatus::FALLING__DOWN);
}

void Player::behit(eDirection direct)
{
	_protectTime = PROTECT_TIME;

	if (direct == NONE)
	{
		_info->SetPlayerHitPoint(_info->GetPlayerHitPoint() - 2);
		return;
	}

	if (this->isInStatus(eStatus::BEING_HIT))
		return;

	this->setStatus(eStatus::BEING_HIT);

	auto move = (Movement*)this->_componentList["Movement"];

	auto g = (Gravity*)this->_componentList["Gravity"];
	g->setStatus(eGravityStatus::FALLING__DOWN);

	switch (direct)
	{
		case RIGHT:
			{
				if (this->getScale().x > 0)
					this->setScaleX(this->getScale().x * (-1));
				move->setVelocity(GVector2(MOVE_SPEED, JUMP_VEL / 1.5));
				break;
			}
		case LEFT:
			{
				if (this->getScale().x < 0)
					this->setScaleX(this->getScale().x * (-1));
				move->setVelocity(GVector2(-MOVE_SPEED, JUMP_VEL / 1.5));
				break;
			}
		default:
			{
				if (this->getScale().x > 0)
					move->setVelocity(GVector2(-MOVE_SPEED, JUMP_VEL / 1.5));
				else
					move->setVelocity(GVector2(MOVE_SPEED, JUMP_VEL / 1.5));
				break;
			}
	}

	//SoundManager::getInstance()->Play(eSoundId::JUMP_SOUND);
}


void Player::sitDown()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, move->getVelocity().y));
}

void Player::falling()
{
	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING__DOWN);
}

void Player::hit()
{
	if (this->isInStatus(BEING_HIT))
		return;
	_isAttacking = true;
	this->removeStatus(MOVING_LEFT);
	this->removeStatus(MOVING_RIGHT);
	if (this->isInStatus(JUMPING))
		this->addStatus(eStatus::ATTACKING);
	this->addStatus(eStatus::ATTACKING);
	this->_animations[this->getStatus()]->restart();

	if (this->_input->isKeyDown(DIK_U) && _info->GetCurrentWeapon() != WEAPON && _listWeapon.size() < _info->GetMaxWeapon())
	{
		_isRope = false;
		_weaponStopWatch->restart();
		_weaponStopWatch->isTimeLoop(0);
	}
	else
	{
		_isRope = true;
		this->_rope->restart();
	}

	_attackStopWatch->restart();
	_attackStopWatch->isStopWatch(ATTACK_TIME);
	if (!SoundManager::getInstance()->IsPlaying(HIT_SOUND))
		SoundManager::getInstance()->Play(HIT_SOUND);
}

void Player::revive()
{
	this->setStage(this->_reviveStage);
	this->_isChangedStage = true;
	this->_isRevive = true;
	_info->SetLife(_info->GetLife() - 1);
	_animations[DIE]->restart();
}

void Player::die()
{
	if (!this->isInStatus(eStatus::DIE))
		this->setStatus(eStatus::DIE);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(-MOVE_SPEED * (this->getScale().x / SCALE_FACTOR), JUMP_VEL));

	auto g = (Gravity*)this->_componentList["Gravity"];
	g->setStatus(eGravityStatus::FALLING__DOWN);
}

void Player::setStatus(eStatus status)
{
	_status = status;
}

RECT Player::getBounding()
{
	int offset = 16;

	RECT bound = _sprite->getBounding();

	if (this->getStatus() != NORMAL)
	{
		if ((this->getStatus() & eStatus::SIT_DOWN) == eStatus::SIT_DOWN)
		{
			bound.top -= offset;
		}
	}

	return bound;
}

GVector2 Player::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}

void Player::forceMoveRight()
{
	onKeyPressed(new KeyEventArg(DIK_K));
}

void Player::unforceMoveRight()
{
	onKeyReleased(new KeyEventArg(DIK_K));
}

void Player::forceMoveLeft()
{
	onKeyPressed(new KeyEventArg(DIK_H));
}

void Player::unforceMoveLeft()
{
	onKeyReleased(new KeyEventArg(DIK_H));
}

void Player::forceJump()
{
	onKeyPressed(new KeyEventArg(DIK_X));
}

void Player::unforceJump()
{
	onKeyReleased(new KeyEventArg(DIK_X));
}

void Player::removeGravity()
{
	auto graivity = (Gravity*)(this->_componentList.find("Gravity")->second);
	graivity->setGravity(VECTOR2ZERO);
}

void Player::unhookinputevent()
{
	if (_input != nullptr)
		__unhook(_input);
}

float Player::getMovingSpeed()
{
	return _movingSpeed;
}

void safeCheckCollision(BaseObject* activeobj, BaseObject* passiveobj, float dt)
{
	if (activeobj != nullptr && passiveobj != nullptr)
	{
		activeobj->checkCollision(passiveobj, dt);
	}
}

bool Player::isChangedStage()
{
	return _isChangedStage;
}

void Player::setStage(eID id)
{
	_currentStage = id;

	if (((int)id) >= 24)
		_mapDirect = LEFT;
	else
		_mapDirect = RIGHT;

	_isChangedStage = false;
}

eID Player::getStage()
{
	return _currentStage;
}

bool Player::IsPlayingMove()
{
	return _isPlayingMovie;
}

eDirection Player::getMapDirection()
{
	return _mapDirect;
}

void Player::StopMovie()
{
	_isPlayingMovie = false;
}

void Player::StartMovieMove()
{
	_movieStartMove = true;
}

bool Player::GetCross()
{
	return _cross;
}

void Player::UseCross()
{
	_cross = false;
}

bool Player::EndLevel()
{
	return _endLevel;
}

Info* Player::GetInfo()
{
	return _info;
}
