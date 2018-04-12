#include "StageManager.h"

StageManager* StageManager::_instance = nullptr;
TileMap* StageManager::_tileMap = nullptr;

StageManager::StageManager(void)
{
}

StageManager::~StageManager(void)
{
}

StageManager* StageManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new StageManager();
	return _instance;
}

list<BaseObject*>* StageManager::getListObject(eID id)
{
	return GetListObjectFromFile(_resourcePath[id]);
}

TileMap* StageManager::getTileMap(eID id)
{
	_tileMap = TileMap::LoadFromFile(_resourcePath[id], id);
	return _tileMap;
}

void StageManager::loadResource()
{
	_resourcePath[eID::MAP_STAGE_10] = "Resources//Map//stage10.tmx";
	_resourcePath[eID::MAP_STAGE_12] = "Resources//Map//stage12.tmx";
}

void StageManager::release()
{
	
}

TileMap* StageManager::getCurrentTileMap()
{
	return  _tileMap;
}
