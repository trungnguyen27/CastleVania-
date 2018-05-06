#include "MonkeyWall.h"


MonkeyWall::MonkeyWall(int x, int y, int width, int height) : BaseObject(eID::MONKEY_WALL)
{
	this->_bound.left = x;
	this->_bound.bottom = y;
	this->_bound.top = y + height;
	this->_bound.right = x + width;
	BaseObject::setPhysicsBodySide(eDirection::ALL);
}

void MonkeyWall::init()
{
	this->_sprite = nullptr;
	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;
}

void MonkeyWall::update(float deltatime)
{
	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}
}

void MonkeyWall::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	return;
}

void MonkeyWall::release()
{
	for (auto it : _listComponent)
	{
		SAFE_DELETE(it.second);
	}
	_listComponent.clear();
}

GVector2 MonkeyWall::getPosition()
{
	return GVector2(_bound.left, _bound.top);
}

float MonkeyWall::getPositionX()
{
	return _bound.left;
}

float MonkeyWall::getPositionY()
{
	return _bound.top;
}

void MonkeyWall::setPosition(GVector3 vector)
{
	_bound.left = vector.x;
	_bound.top = vector.y;
}

void MonkeyWall::setPosition(float x, float y, float z)
{
	_bound.left = x;
	_bound.top = y;
}

void MonkeyWall::setPosition(GVector2 position)
{
	_bound.left = position.x;
	_bound.top = position.y;
}

void MonkeyWall::setPosition(float x, float y)
{
	_bound.left = x;
	_bound.top = y;
}

void MonkeyWall::setPositionX(float x)
{
	_bound.left = x;
}

void MonkeyWall::setPositionY(float y)
{
	_bound.right = y;
}

GVector2 MonkeyWall::getScale()
{
	return VECTOR2ONE;
}

void MonkeyWall::setScale(GVector2 scale)
{
	// do nothing.
}

void MonkeyWall::setScale(float scale)
{
	// do nothing.
}

void MonkeyWall::setScaleX(float sx)
{
	// do nothing.
}

void MonkeyWall::setScaleY(float sy)
{
	// do nothing.
}

GVector2 MonkeyWall::getOrigin()
{
	return VECTOR2ZERO;
}

void MonkeyWall::setOrigin(GVector2 origin)
{
	// do nothing.
}

float MonkeyWall::getRotate()
{
	return 0.0f;
}

void MonkeyWall::setRotate(float degree)
{
	// do nothing.
}

void MonkeyWall::setZIndex(float z)
{
	// do nothing.
}

float MonkeyWall::getZIndex()
{
	return 1; // hard code
}

GVector2 MonkeyWall::getAnchorPoint()
{
	return GVector2(_bound.left, _bound.right);
}

RECT MonkeyWall::getBounding()
{
	return _bound;
}

MonkeyWall::~MonkeyWall()
{
}
