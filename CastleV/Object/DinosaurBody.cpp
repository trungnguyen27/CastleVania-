#include "DinosaurBody.h"

DinosaurBody::DinosaurBody(int x, int y) :BaseObject(DINOSAUR)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ENEMY);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ENEMY, "dinosaur_3"));
	_sprite->setPosition(x, y);
	_animation = new Animation(_sprite, 0.1f);
	_animation->addFrameRect(ENEMY, "dinosaur_3", NULL);
	_animation->setValueFlashes(0.5);

	//_effect = SpriteManager::getInstance()->getSprite(eID::EFFECT);
	//_effect->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EFFECT, "hit_effect_1"));
	//_effectAnimation = new Animation(_effect, 0.15f);
	//_effectAnimation->addFrameRect(EFFECT, "hit_effect_1", "hit_effect_2", "hit_effect_3", "hit_effect_4", NULL);
	//_effect->setPosition(this->getPosition());

	_hitPoint = 6;
	_direct = false;
	_startHit = false;
	_ready = 0;
}

void DinosaurBody::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	_animation->draw(spriteHandle, viewport);
}

void DinosaurBody::shoot()
{

}

void DinosaurBody::setDirect(bool direct)
{
	_direct = direct;
}


void DinosaurBody::update(float deltatime)
{
	_animation->update(deltatime);
	if (!_ready && _readyStopWatch->isStopWatch(500))
	{
		_ready = true;
		_animation->enableFlashes(false);
		_readyStopWatch->restart();
	}

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}

void DinosaurBody::release()
{
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	SAFE_DELETE(_sprite);
	SAFE_DELETE(_animation);
	_componentList.clear();
}

void DinosaurBody::Destroy()
{
	BaseObject* heart = new Heart(this->getPositionX(), this->getPositionY() + 32);

	if (heart != nullptr)
	{
		heart->init();
		QuadTreeNode::getInstance()->Insert(heart);
	}
}

void DinosaurBody::init()
{
	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;

	_effectStopWatch = new StopWatch();
	_hitStopWatch = new StopWatch();
	_readyStopWatch = new StopWatch();
	_shoot1StopWatch = new StopWatch();
	_shoot2StopWatch = new StopWatch();

	_readyStopWatch->isTimeLoop(0);
	_shoot1StopWatch->isTimeLoop(600);
	_shoot2StopWatch->isTimeLoop(1200);
}

bool DinosaurBody::isDead()
{
	return (_hitPoint <= 0);
}

void DinosaurBody::wasHit(int hitpoint)
{
	_ready = false;
	_animation->enableFlashes(true);
}
