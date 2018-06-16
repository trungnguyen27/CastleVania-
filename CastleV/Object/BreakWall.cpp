#include "BreakWall.h"

BreakWall::BreakWall(int x, int y, eID item) : BaseObject(BREAK_WALL)
{
	_item = item;

	_sprite = SpriteManager::getInstance()->getSprite(eID::WALL);
	_sprite->setPosition(x, y);
	_sprite->setScale(2.0);

	_effect = SpriteManager::getInstance()->getSprite(eID::BREAK_WALL);
	_effect->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BREAK_WALL, "break_1"));
	_effect->setPosition(x, y);
	_effectAnimation = new Animation(_effect, 0.08f);
	_effectAnimation->addFrameRect(BREAK_WALL, "break_1", "break_2", "break_3", "break_4", "break_5", "break_6", "break_7", "break_8", "break_9", "break_10", NULL);

	_wasHit = false;
}

void BreakWall::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_wasHit)
		_effectAnimation->draw(spriteHandle, viewport);
	else
		_sprite->render(spriteHandle, viewport);
}

void BreakWall::update(float deltatime)
{
	if (_wasHit)
	{
		_effectAnimation->update(deltatime);

		if (_effectStopWatch->isStopWatch(600))
		{
			this->setStatus(DESTROY);
			BaseObject* item = nullptr;

			//if (_item == INCREASE)
			//	item = new IncreaseWeapon(this->getPositionX(), this->getPositionY());
			//else if (_item == CHICKEN)
			//	item = new Chicken(this->getPositionX(), this->getPositionY());
			//else if (_item != ITEM)
			//	item = new Heart(this->getPositionX(), this->getPositionY());
			item = new Heart(this->getPositionX(), this->getPositionY());
			if (item != nullptr)
			{
				item->init();
				QuadTreeNode::getInstance()->Insert(item);
			}
		}
	}

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}

void BreakWall::release()
{
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();
}

void BreakWall::init()
{
	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;

	_effectStopWatch = new StopWatch();
}

RECT BreakWall::getBounding()
{
	return _sprite->getBounding();
}

void BreakWall::wasHit()
{
	_wasHit = true;
	_effectStopWatch->isTimeLoop(600);
}

bool BreakWall::isBroken()
{
	return (_wasHit);
}
