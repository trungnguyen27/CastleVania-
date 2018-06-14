#include "Dinosaur.h"
#include <time.h>  
Dinosaur::Dinosaur(int x, int y) :BaseObject(DINOSAUR)
{
	x = x - 20;
	_sprite = SpriteManager::getInstance()->getSprite(eID::ENEMY);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ENEMY, "dinosaur_1"));
	_sprite->setPosition(x , y);
	_animation = new Animation(_sprite, 0.1f);
	_animation->addFrameRect(ENEMY, "dinosaur_1", NULL);
	_animation->setValueFlashes(0.5);
	_headInitialPos = GVector2(x, y);
	_bodyInitialPos = GVector2(x + 20, y);
	_status = DinosaurStatus::IDLE;

	_hitPoint = 6;
	_direct = false;
	_startHit = false;
	_ready = 0;
}

void Dinosaur::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	//for (auto body : bodyParts)
	//{
	//	body->draw(spriteHandle, viewport);
	//}
	_animation->draw(spriteHandle, viewport);
}

void Dinosaur::shoot()
{
	if (_shootingTarget)
	{
		bool _isLeft = true;
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

void Dinosaur::setShootingTarget(BaseObject* target)
{
	_shootingTarget = target;
}

void Dinosaur::setDirect(bool direct)
{
	_direct = direct;
}


void Dinosaur::update(float deltatime)
{
	_animation->update(deltatime);

	if (_hitPoint > 0)
	{
		if (_startHit)
		{
			if (_hitStopWatch->isStopWatch(400))
			{
				_startHit = false;
				_animation->enableFlashes(false);
				_hitStopWatch->restart();
				SoundManager::getInstance()->Stop(HIT_DINOSAUR);
				SoundManager::getInstance()->Play(HIT_DINOSAUR);
			}
		}

		if (_shoot1StopWatch->isTimeLoop(SHOOT_DELAY))
		{
			this->shoot();
		}

		if (_shoot2StopWatch->isTimeLoop(SHOOT_DELAY))
		{
			this->shoot();
		}
		if (_goForwardStopWatch->isStopWatch(3000))
		{
			stop();
		}

		if (abs(this->getPositionX() - _headInitialPos.x)> BODY_LENGTH && this->getStatus() == DinosaurStatus::FORWARD)
		{
			_status = DinosaurStatus::DOWN;
			_lastYPos = this->getPositionY();
		}
		/*
		if (this->getPositionY() - _initialPos.y > UP_DISTANCE && this->getStatus() != DinosaurStatus::DOWN)
		{
			_status = DinosaurStatus::DOWN;
			_lastYPos = this->getPositionY();
		}
		
		if (_initialPos.y - this->getPositionY()> DOWN_DISTANCE &&  this->getStatus() != DinosaurStatus::UP)
		{
			_status = DinosaurStatus::UP;
			_lastYPos = this->getPositionY();
		}*/
	}
	else
	{
		//_effectAnimation->update(deltatime);
		if (_effectStopWatch->isStopWatch(600))
		{
			this->setStatus(DESTROY);

			for (auto part : bodyParts)
			{
				((DinosaurBody*)part)->Destroy();
			}

			srand(time(0));
			auto ran = rand() % 10;
			BaseObject* heart = nullptr;
			if (ran < 3)
				heart = new Heart(this->getPositionX(), this->getPositionY() + 32);
			else
				heart = new Heart(this->getPositionX(), this->getPositionY() + 32);

			if (heart != nullptr)
			{
				heart->init();
				QuadTreeNode::getInstance()->Insert(heart);
			}
		}
	}
	this->updateStatus(deltatime);
	//for (auto part : bodyParts)
	//{
	//	part->update(deltatime);
	//}
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}

void Dinosaur::updateStatus(float dt)
{
	float gap = abs(this->_lastYPos - this->getPositionY());
	if (gap < UP_DISTANCE)
	{
		gap = UP_DISTANCE / 2;
	}
	if (this->getStatus() == DinosaurStatus::FORWARD)
	{
		this->goForward(_initialVelocity);
	}
	else {
		move(dt);
	}
	/*else if (this->getStatus() == DinosaurStatus::DOWN) {
		
		goDown(gap);
	}
	else if (this->getStatus() == DinosaurStatus::UP) {

		goUp(gap);
	}*/
}

void Dinosaur::move(float deltatime)
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));
	elapsedTime += deltatime/5000;
	float frequency = 8;
	float waveLength = 40;
	float newTailPos = sin(frequency*elapsedTime + (BODY_PARTS - 1) / 1.5f) * waveLength;
	float newHeadPos = _headInitialPos.y + sin(frequency*elapsedTime) * waveLength - newTailPos;
	if (abs(newHeadPos - this->getPositionY()) > 10)
		this->setPositionY(newHeadPos);
	//this->setPositionY(_initialPos.y + sin(2 * 3.14f * elapsedTime) * 25);
	for (int i = 0; i < bodyParts.size() ; i++) {
		auto part = bodyParts[i];
		float newYPos = sin(frequency*elapsedTime + (i + 1) / 1.5f) * waveLength;
		float pos = _bodyInitialPos.y + newYPos - newTailPos;
		if (abs(pos - part->getPositionY()) > 10)
			part->setPositionY(pos);
	}
	bodyParts[bodyParts.size() - 1]->setPositionY(_bodyInitialPos.y);
}


void Dinosaur::goUp(float gap) {
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, MOVE_SPEED));
	int partBreak = BODY_PARTS/2 + (int)(UP_DISTANCE/gap*1.0f);
	int index;
	if (partBreak == 0)
		index = 0;
	else if (partBreak >= BODY_PARTS) {
		index = BODY_PARTS - 1;
	}
	else {
		index = partBreak - 1;
	}
	float step = (float)(bodyParts[index]->getPositionY() - this->getPositionY()) / (index);
	for (int i = 0; i < index; i++) {
		auto part = bodyParts[i];
		float newYPosition = step*i + this->getPositionY();
		part->setPositionY(newYPosition);
	}
}
void Dinosaur::goDown(float gap) {
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, -MOVE_SPEED));
	int partBreak = (int)(gap / (DOWN_DISTANCE*1.0f) * BODY_PARTS);
	int index;
	if (partBreak == 0)
		index = 0;
	else if (partBreak >= BODY_PARTS) {
		index = BODY_PARTS - 1;
	}
	else {
		index = partBreak - 1;
	}
	float step = (float)(bodyParts[index]->getPositionY() - this->getPositionY()) / (index);
	for (int i = 0; i < index; i++) {
		auto part = bodyParts[i];
		float newYPosition = step*i + this->getPositionY();
		part->setPositionY(newYPosition);
	}
}
void Dinosaur::goForward(GVector2 direction) {
	auto pos = this->getPosition();
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(direction);
	// distance of head and tail
	// Distribute
	float stepX = (float)(_bodyInitialPos.x - this->getPositionX())/(BODY_PARTS);
	float stepY = (float)(_bodyInitialPos.y - this->getPositionY()) / (BODY_PARTS);
	for (int i = 0; i < BODY_PARTS; i++) {
		auto part = bodyParts[i];
		float newXPosition = stepX*(i) + this->getPositionX();
		float newYPosition = stepY*(i)+this->getPositionY();
		part->setPosition(GVector2(newXPosition + 16, newYPosition));
	}
}

void Dinosaur::stop()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0,0));
}

void Dinosaur::release()
{
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}

	for (auto body : bodyParts)
	{
		SAFE_DELETE(body);
	}
	bodyParts.clear();
	_componentList.clear();
}

void Dinosaur::Active(bool active)
{
	if (!_active) {
		_active = active;
		if (_active)
			srand(time(NULL));
			int random = rand() % 60;
			int ranDir = rand() % 2;
			if (ranDir == 0)
				ranDir = -1;
			else ranDir = 1;
			_initialVelocity = GVector2(-MOVE_SPEED, ranDir*random);
			_status = DinosaurStatus::FORWARD;
	}
}

void Dinosaur::init()
{
	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;

	auto movement = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList["Movement"] = movement;

	_effectStopWatch = new StopWatch();
	_hitStopWatch = new StopWatch();
	_readyStopWatch = new StopWatch();
	_shoot1StopWatch = new StopWatch();
	_shoot2StopWatch = new StopWatch();
	_goForwardStopWatch = new StopWatch();
	_hitPoint = 5;

	_readyStopWatch->isTimeLoop(0);
	_shoot1StopWatch->isTimeLoop(600);
	_shoot2StopWatch->isTimeLoop(1200);

	for (int i = 1; i <= BODY_PARTS; i++)
	{
		auto body = new DinosaurBody(this->getPositionX() + 20, this->getPositionY());
		body->init();
		bodyParts.push_back(body);
		//QuadTreeNode::getInstance()->Insert(body);
	}
}

bool Dinosaur::isDead()
{
	return (_hitPoint <= 0);
}

void Dinosaur::wasHit(int hitpoint)
{
	if (!_startHit)
	{
		_hitPoint -= hitpoint;
		_hitStopWatch->restart();
		_hitStopWatch->isTimeLoop(400);
		_startHit = true;
		_animation->enableFlashes(true);
		for (auto part : bodyParts)
		{
			((DinosaurBody*)part)->wasHit(0);
		}
	}
}
