#include "Info.h"
#include "TextSprite.h"
#include <string>
#include <sstream>
#include <iomanip>

eID Info::GetCurrentWeapon()
{
	return _weaponID;
}

void Info::SetWeapon(eID id)
{
	_weaponID = id;
}


Info::Info() : BaseObject(eID::INFO)
{
	_weaponSprite = nullptr;
	_maxWeaponSprite = nullptr;
	_maxWeapon = 1;

	_border = SpriteManager::getInstance()->getSprite(BORDER);
	_border->setScale(0.8f);
	_border->setPosition(300, 42);
}

Info::~Info()
{
}

void Info::init()
{
}

void Info::update(float deltatime)
{
	
}

void Info::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	_border->render(spriteHandle);

	if (_weaponSprite != nullptr)
		_weaponSprite->render(spriteHandle);

	if (_maxWeaponSprite != nullptr)
		_maxWeaponSprite->render(spriteHandle);

	auto posPlayer = GVector2(100, 34);
	for (int i = 0; i < 16; i++)
	{
		if (i < _playerHitPointNumber)
		{
			/*_iconPlayerHitPoint->setPosition(posPlayer);
			_iconPlayerHitPoint->render(spriteHandle);*/
		}
		else
		{
			/*_iconEmptyHitPoint->setPosition(posPlayer);
			_iconEmptyHitPoint->render(spriteHandle);*/
		}

		posPlayer = posPlayer + GVector2(10, 0);
	}

	/*_textEnemy->draw(spriteHandle);*/
	auto posEnemy = GVector2(100, 53);
	for (int i = 0; i < 16; i++)
	{
		if (i < _enemyHitPointNumber)
		{
			/*_iconEnemyHitPoint->setPosition(posEnemy);
			_iconEnemyHitPoint->render(spriteHandle);*/
		}
		else
		{
		/*	_iconEmptyHitPoint->setPosition(posEnemy);
			_iconEmptyHitPoint->render(spriteHandle);*/
		}

		posEnemy = posEnemy + GVector2(10, 0);
	}
}

void Info::release()
{
	SAFE_DELETE(_iconPlayerHitPoint);
	SAFE_DELETE(_textPlayer);
	SAFE_DELETE(_iconEnemyHitPoint);
	SAFE_DELETE(_textEnemy);
	SAFE_DELETE(_iconHeart);
	SAFE_DELETE(_textHeart);
	SAFE_DELETE(_textLife);
	SAFE_DELETE(_textScore);
	SAFE_DELETE(_textStage);
}

int Info::GetEnemyHitPoint()
{
	return _enemyHitPointNumber;
}

int Info::GetHeart()
{
	return _heartNumber;
}

int Info::GetLife()
{
	return _lifeNumber;
}

int Info::GetPlayerHitPoint()
{
	return _playerHitPointNumber;
}

int Info::GetScore()
{
	return _scoreNumber;
}

int Info::GetStage()
{
	return _stageNumber;
}

int Info::GetTime()
{
	return time;
}


void Info::SetEnemyHitPoint(int number)
{
	_enemyHitPointNumber = number;
}

void Info::SetHeart(int number)
{
	_heartNumber = number;
	if (_heartNumber >= 100)
		_heartNumber = 99;
}

void Info::SetLife(int number)
{
	_lifeNumber = number;
}

void Info::SetPlayerHitPoint(int number)
{
	_playerHitPointNumber = number;
	if (_playerHitPointNumber > 16)
		_playerHitPointNumber = 16;
}

void Info::SetScore(int number)
{
	_scoreNumber = number;
}

void Info::AddScore(int number)
{
	_scoreNumber += number;
}

void Info::SetStage(int number)
{
	_stageNumber = number;
}

void Info::SetTime(int number)
{
	if (!_pauseTime)
	{
		_timeNumber = number + 1;
		_beginTime = GameTime::getInstance()->getTotalGameTime();
		time = number;
	}
	else
		time = number;
}

void Info::SetMaxWeapon(int num)
{
	/*switch(num)
	{
	case 2:
		_maxWeaponSprite = SpriteManager::getInstance()->getSprite(ITEM);
		_maxWeaponSprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "small2"));
		_maxWeaponSprite->setPosition(420, 45);
		break;
	case 3:
		_maxWeaponSprite = SpriteManager::getInstance()->getSprite(ITEM);
		_maxWeaponSprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "small3"));
		_maxWeaponSprite->setPosition(420, 45);
		break;
	default:
		num = 1;
		_maxWeaponSprite = nullptr;
		break;
	}*/
	_maxWeapon = num;
}

int Info::GetMaxWeapon()
{
	return _maxWeapon;
}

void Info::ActiveTime()
{
	_pauseTime = false;
}

void Info::PauseTime()
{
	_pauseTime = true;
}

