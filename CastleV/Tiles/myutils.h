#ifndef __MYUTIL_H__
#define __MYUTIL_H__

#include "../Framework/define.h"
#include "../pugixml/pugixml.hpp"
#include "../Object/BaseObject.h"
#include <list>
#include "../Object/Wall.h"
#include "../Object/Start.h"
#include "../Object/End.h"
#include "../Object/Candle.h"
#include "../Object/Water.h"
#include "../Object/Revive.h"
#include "../Object/FloatingWall.h"
#include <ctime>
using namespace pugi;

map<string, string> GetObjectProperties(xml_node node);
BaseObject* GetStart(xml_node item, int mapHeight);
BaseObject* GetEnd(xml_node item, int mapHeight);
BaseObject* GetWater(xml_node item, int mapHeight);
BaseObject* GetRevive(xml_node item, int mapHeight);
BaseObject* GetFloatingWall(xml_node item, int mapHeight);
BaseObject* GetCandle(xml_node item, int mapHeight);
list<BaseObject*>* GetListObjectFromFile(const string path);

#endif