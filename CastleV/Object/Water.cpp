#include "Water.h"

Water::Water(int x, int y, int width, int height) : BaseObject(eID::WATER)
{
	this->_bound.left = x;
	this->_bound.bottom = y;
	this->_bound.top = y + height;
	this->_bound.right = x + width;
	BaseObject::setPhysicsBodySide(eDirection::NONE);
}

void Water::init()
{
	this->_sprite = nullptr;
	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;
}

void Water::update(float deltatime)
{
	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}
}

void Water::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	return;
}

void Water::release()
{
	for (auto it : _listComponent)
	{
		SAFE_DELETE(it.second);
	}
	_listComponent.clear();
}


GVector2 Water::getPosition()
{
	return GVector2(_bound.left, _bound.bottom);
}

float Water::getPositionX()
{
	return _bound.left;
}

float Water::getPositionY()
{
	return _bound.top;
}

void Water::setPosition(GVector3 vector)
{
	_bound.left = vector.x;
	_bound.top = vector.y;
}

void Water::setPosition(float x, float y, float z)
{
	_bound.left = x;
	_bound.top = y;
}

void Water::setPosition(GVector2 position)
{
	_bound.left = position.x;
	_bound.top = position.y;
}

void Water::setPosition(float x, float y)
{
	_bound.left = x;
	_bound.top = y;
}

void Water::setPositionX(float x)
{
	_bound.left = x;
}

void Water::setPositionY(float y)
{
	_bound.right = y;
}

GVector2 Water::getScale()
{
	return VECTOR2ONE;
}

void Water::setScale(GVector2 scale)
{
	// do nothing.
}

void Water::setScale(float scale)
{
	// do nothing.
}

void Water::setScaleX(float sx)
{
	// do nothing.
}

void Water::setScaleY(float sy)
{
	// do nothing.
}

GVector2 Water::getOrigin()
{
	return VECTOR2ZERO;
}

void Water::setOrigin(GVector2 origin)
{
	// do nothing.
}

float Water::getRotate()
{
	return 0.0f;
}

void Water::setRotate(float degree)
{
	// do nothing.
}

void Water::setZIndex(float z)
{
	// do nothing.
}

float Water::getZIndex()
{
	return 1; // hard code
}

GVector2 Water::getAnchorPoint()
{
	return GVector2(_bound.left, _bound.right);
}

RECT Water::getBounding()
{
	return _bound;
}

Water::~Water()
{
}
