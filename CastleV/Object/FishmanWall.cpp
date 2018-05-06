#include "FishmanWall.h"


FishmanWall::FishmanWall(int x, int y, int width, int height) : BaseObject(eID::FISHMAN_WALL)
{
	this->_bound.left = x;
	this->_bound.bottom = y;
	this->_bound.top = y + height;
	this->_bound.right = x + width;
	BaseObject::setPhysicsBodySide(eDirection::ALL);
}

void FishmanWall::init()
{
	this->_sprite = nullptr;
	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;
}

void FishmanWall::update(float deltatime)
{
	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}
}

void FishmanWall::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	return;
}

void FishmanWall::release()
{
	for (auto it : _listComponent)
	{
		SAFE_DELETE(it.second);
	}
	_listComponent.clear();
}

GVector2 FishmanWall::getPosition()
{
	return GVector2(_bound.left, _bound.top);
}

float FishmanWall::getPositionX()
{
	return _bound.left;
}

float FishmanWall::getPositionY()
{
	return _bound.top;
}

void FishmanWall::setPosition(GVector3 vector)
{
	_bound.left = vector.x;
	_bound.top = vector.y;
}

void FishmanWall::setPosition(float x, float y, float z)
{
	_bound.left = x;
	_bound.top = y;
}

void FishmanWall::setPosition(GVector2 position)
{
	_bound.left = position.x;
	_bound.top = position.y;
}

void FishmanWall::setPosition(float x, float y)
{
	_bound.left = x;
	_bound.top = y;
}

void FishmanWall::setPositionX(float x)
{
	_bound.left = x;
}

void FishmanWall::setPositionY(float y)
{
	_bound.right = y;
}

GVector2 FishmanWall::getScale()
{
	return VECTOR2ONE;
}

void FishmanWall::setScale(GVector2 scale)
{
	// do nothing.
}

void FishmanWall::setScale(float scale)
{
	// do nothing.
}

void FishmanWall::setScaleX(float sx)
{
	// do nothing.
}

void FishmanWall::setScaleY(float sy)
{
	// do nothing.
}

GVector2 FishmanWall::getOrigin()
{
	return VECTOR2ZERO;
}

void FishmanWall::setOrigin(GVector2 origin)
{
	// do nothing.
}

float FishmanWall::getRotate()
{
	return 0.0f;
}

void FishmanWall::setRotate(float degree)
{
	// do nothing.
}

void FishmanWall::setZIndex(float z)
{
	// do nothing.
}

float FishmanWall::getZIndex()
{
	return 1; // hard code
}

GVector2 FishmanWall::getAnchorPoint()
{
	return GVector2(_bound.left, _bound.right);
}

RECT FishmanWall::getBounding()
{
	return _bound;
}

FishmanWall::~FishmanWall()
{
}
