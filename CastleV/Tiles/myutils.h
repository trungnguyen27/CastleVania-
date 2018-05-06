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
#include "../Object/Bat.h"
#include "../Object/Fishman.h"
#include "../Object/FishmanWall.h"
#include "../Object/YellowBag.h"
#include "../Object/RedBag.h"
#include "../Object/Heart.h"
#include "../Object/BigHeart.h"
#include "../Object/HolyWater.h"
#include "../Object/Dagger.h"
#include "../Object/Food.h"
#include "../Object/StopTime.h"
#include "../Object/Frankenstein.h"
#include "../Object/Dinosaur.h"
#include "../Object/FireBall.h"
#include "../Object/Monkey.h"
#include "../Object/MonkeyWall.h"
#include <ctime>
using namespace pugi;

map<string, string> GetObjectProperties(xml_node node);
BaseObject* GetStart(xml_node item, int mapHeight);
BaseObject* GetEnd(xml_node item, int mapHeight);
BaseObject* GetWater(xml_node item, int mapHeight);
BaseObject* GetRevive(xml_node item, int mapHeight);
BaseObject* GetFloatingWall(xml_node item, int mapHeight);
BaseObject* GetCandle(xml_node item, int mapHeight);
BaseObject* GetBat(xml_node item, int mapHeight);
BaseObject* GetFishman(xml_node item, int mapHeight);
BaseObject* GetWall(xml_node item, int mapHeight);
BaseObject* GetFishmanWall(xml_node item, int mapHeight);
BaseObject* GetMonkeyWall(xml_node item, int mapHeight);
BaseObject* GetYellowBag(xml_node item, int mapHeight);
BaseObject* GetRedBag(xml_node item, int mapHeight);
BaseObject* GetHeart(xml_node item, int mapHeight);
BaseObject* GetBigHeart(xml_node item, int mapHeight);
BaseObject* GetStopTime(xml_node item, int mapHeight);
BaseObject* GetHolyWater(xml_node item, int mapHeight);
BaseObject* GetFrankenstein(xml_node item, int mapHeight);
BaseObject* GetMonkey(xml_node item, int mapHeight);
BaseObject* GetDinosaur(xml_node item, int mapHeight);
list<BaseObject*>* GetListObjectFromFile(const string path);

#endif