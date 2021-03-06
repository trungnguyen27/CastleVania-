﻿#include "SpriteManager.h"
#include "../pugixml/pugixml.hpp"
SpriteManager* SpriteManager::_instance = nullptr;

SpriteManager::SpriteManager(void)
{
	// do nothing.
}

SpriteManager::~SpriteManager(void)
{
	for (auto spr = _listSprite.begin(); spr != _listSprite.end(); ++spr)
	{
		spr->second->release(); // release image
		delete spr->second; // delete sprite
	}
	if (_listSprite.empty() == false)
		_listSprite.clear(); // remove all from MAP
}

void SpriteManager::loadResource(LPD3DXSPRITE spriteHandle)
{
	Sprite* pSprite = NULL;

	pSprite = new Sprite(spriteHandle, L"Resources//Images//border.png");
	this->_listSprite[eID::BORDER] = pSprite;
	pSprite = new Sprite(spriteHandle, L"Resources//Images//floating_wall.png");
	this->_listSprite[eID::FLOATING_WALL] = pSprite;
	this->loadSpriteInfo(eID::FLOATING_WALL, "Resources//Images//floating_wall.txt");

	pSprite = new Sprite(spriteHandle, L"Resources//Images//breakwall.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::BREAK_WALL, pSprite));
	this->loadSpriteInfo(eID::BREAK_WALL, "Resources//Images//breakwall.txt");

	pSprite = new Sprite(spriteHandle, L"Resources//Images//wall_cube.png");
	this->_listSprite[eID::WALL] = pSprite;

	pSprite = new Sprite(spriteHandle, L"Resources//Images//simon.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::PLAYER, pSprite));
	this->loadSpriteInfo(eID::PLAYER, "Resources//Images//simon.txt");

	pSprite = new Sprite(spriteHandle, L"Resources//Images//enemy.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::ENEMY, pSprite));
	this->loadSpriteInfo(eID::ENEMY, "Resources//Images//enemy.txt");

	pSprite = new Sprite(spriteHandle, L"Resources//Images//items.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::ITEM, pSprite));
	this->loadSpriteInfo(eID::ITEM, "Resources//Images//items.txt");

	pSprite = new Sprite(spriteHandle, L"Resources//Images//weapon.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::WEAPON, pSprite));
	this->loadSpriteInfo(eID::WEAPON, "Resources//Images//weapon.txt");

	pSprite = new Sprite(spriteHandle, L"Resources//Images//candle.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::CANDLE, pSprite));
	this->loadSpriteInfo(eID::CANDLE, "Resources//Images//candle.txt");

	pSprite = new Sprite(spriteHandle, L"Resources//Images//rope.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::ROPE, pSprite));
	this->loadSpriteInfo(eID::ROPE, "Resources//Images//rope.txt");

	pSprite = new Sprite(spriteHandle, L"Resources//Font//fontFull.png", 54, 6);
	this->_listSprite[eID::FONTFULL] = pSprite;

	pSprite = new Sprite(spriteHandle, L"Resources//Images//healing.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::HEALING, pSprite));
	this->loadSpriteInfo(eID::HEALING, "Resources//Images//healing.txt");

	pSprite = new Sprite(spriteHandle, L"Resources//Images//heart.png");
	this->_listSprite[eID::HEART_ICON] = pSprite;

	pSprite = new Sprite(spriteHandle, L"Resources//Images//effects.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::EFFECT, pSprite));
	this->loadSpriteInfo(eID::EFFECT, "Resources//Images//effects.txt");

	pSprite = loadXMLDoc(spriteHandle, L"Resources//Map//stage10.tmx");
	pSprite->setOrigin(VECTOR2ZERO);
	pSprite->setScale(2.0f);
	this->_listSprite[eID::MAP_STAGE_10] = pSprite;

	pSprite = loadXMLDoc(spriteHandle, L"Resources//Map//stage12.tmx");
	pSprite->setOrigin(VECTOR2ZERO);
	pSprite->setScale(2.0f);
	this->_listSprite[eID::MAP_STAGE_12] = pSprite;

}

Sprite* SpriteManager::loadXMLDoc(LPD3DXSPRITE spritehandle, LPWSTR path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path, pugi::parse_default | pugi::parse_pi);
	if (result == false)
	{
		OutputDebugString(L"Khong tim thay file");
		return nullptr;
	}
	auto tileset_node = doc.child("map").child("tileset");
	int tilecount = tileset_node.attribute("tilecount").as_int();
	int columns = tileset_node.attribute("columns").as_int();
	auto image = tileset_node.child("image");

	string filename = image.attribute("source").as_string();
	wstring L_filename = wstring(filename.begin(), filename.end());
	wstring strpath = wstring(path);
	int index = strpath.find_last_of(L'//');
	strpath = strpath.substr(0, index);
	strpath += L"/" + L_filename;

	return new Sprite(spritehandle, (LPWSTR) strpath.c_str(), tilecount, columns);
}

Sprite* SpriteManager::getSprite(eID id)
{
	Sprite* it = this->_listSprite.find(id)->second;
	return new Sprite(*it); // get the copy version of Sprite
}

RECT SpriteManager::getSourceRect(eID id, string name)
{
	//return _sourceRectList[id].at(name);
	return _sourceRectList[id][name];
}

void SpriteManager::loadSpriteInfo(eID id, const char* fileInfoPath)
{
	FILE* file;
	file = fopen(fileInfoPath, "r");

	if (file)
	{
		while (!feof(file))
		{
			RECT rect;
			char name[100];
			fgets(name, 100, file);

			fscanf(file, "%s %d %d %d %d", &name, &rect.left, &rect.top, &rect.right, &rect.bottom);

			_sourceRectList[id][string(name)] = rect;
		}
	}

	fclose(file);
}

void SpriteManager::releaseSprite(eID id)
{
	Sprite* it = this->_listSprite.find(id)->second;
	delete it; // delete the sprite only, dont relase image
	this->_listSprite.erase(id); // erase funciotn only remove the pointer from MAP, dont delete it.
}

void SpriteManager::releaseTexture(eID id)
{
	Sprite* spr = this->_listSprite.find(id)->second;
	spr->release(); // release image
	delete spr;
	this->_listSprite.erase(id); // erase funciotn only remove the pointer from MAP, dont delete it.
}

SpriteManager* SpriteManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new SpriteManager();
	return _instance;
}

void SpriteManager::release()
{
	delete _instance; // _instance is static attribute, only static function can delete it.
	_instance = nullptr;
}
