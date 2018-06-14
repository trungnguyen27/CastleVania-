#include "HolyWaterWeapon.h"

HolyWaterWeapon::HolyWaterWeapon(int x, int y, bool direct) : Weapon(HOLYWATER)
{
	_comeBack = false;
	_sprite = SpriteManager::getInstance()->getSprite(ITEM);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "holy_water"));
	_sprite->setPosition(x, y);

	auto move = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList["Movement"] = move;

	if (direct)
	{
		move->setVelocity(GVector2(HOLYWATER_FLYING_SPEED, 300));
		move->setAccelerate(GVector2(-100,-800));
	}
	else
	{
		_sprite->setScaleX(-1);
		move->setVelocity(GVector2(-HOLYWATER_FLYING_SPEED, 300));
		move->setAccelerate(GVector2(100, -800));
	}
}

void HolyWaterWeapon::ComeBack()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(move->getVelocity().x*(-1), move->getVelocity().y));
	_comeBack = true;
}


bool HolyWaterWeapon::IsBoomerangComeBack()
{
	return _comeBack;
}


void HolyWaterWeapon::release()
{
}

void HolyWaterWeapon::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	_sprite->render(spriteHandle, viewport);
}

CollisionBody* HolyWaterWeapon::getCollisionBody()
{
	return (CollisionBody*)_componentList["CollisionBody"];
}

void HolyWaterWeapon::init()
{
	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;
}

void HolyWaterWeapon::update(float deltatime)
{
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}
