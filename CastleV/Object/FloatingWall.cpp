#include "FloatingWall.h"

void FloatingWall::release()
{
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();
}

FloatingWall::FloatingWall(int x, int y, int activeX, bool reversed, bool isquadtree = true) : BaseObject(FLOATING_WALL)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::FLOATING_WALL);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::FLOATING_WALL, "floating_wall"));
	_sprite->setPosition(x, y);
	if (reversed) {
		_activeXLeft = x;
		_activeXRight = activeX;
	}
	else {
		_activeXLeft = activeX;
		_activeXRight = x;
	}

	setQuadTree(isquadtree);
}

void FloatingWall::init()
{
	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;

	auto movement = new Movement(GVector2(0, 0), GVector2(-WALL_MOVE_SPEED, 0), _sprite);
	_componentList["Movement"] = movement;
}

void FloatingWall::update(float deltatime)
{
	auto move = (Movement*)this->_componentList["Movement"];
	if (this->getPositionX() < _activeXLeft)
	{
		move->setVelocity(GVector2(WALL_MOVE_SPEED, 0));
	}
	else if (this->getPositionX() > _activeXRight)
	{
		move->setVelocity(GVector2(-WALL_MOVE_SPEED, 0));
	}

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}

void FloatingWall::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	_sprite->render(spriteHandle, viewport);
}

GVector2 FloatingWall::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}
