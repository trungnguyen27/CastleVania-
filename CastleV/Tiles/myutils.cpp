#include "myutils.h"

map<string, string> GetObjectProperties(xml_node node)
{
	map<string, string> properties;

	properties["x"] = node.attribute("x").as_string();
	properties["y"] = node.attribute("y").as_string();
	properties["width"] = node.attribute("width").as_string();
	properties["height"] = node.attribute("height").as_string();

	// parameters
	auto params = node.child("properties");
	if (params)
	{
		for (auto item : params)
		{
			auto key = item.attribute("name").as_string();
			auto value = item.attribute("value").as_string();
			properties[key] = value;
		}
	}

	return properties;
}
//
BaseObject* GetEnd(xml_node item, int mapHeight)
{
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;

	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;


	auto end = new End(x, y, width, height);
	end->setNextStage((eID)stoi(properties["next"]));
	end->setStatus((eStatus)stoi(properties["status"]));
	return end;
}

BaseObject* GetWater(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;

	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	auto water = new Water(x, y, width, height);
	water->setStatus((eStatus)stoi(properties["status"]));
	water->init();
	return water;
}
//BaseObject* GetBack(xml_node item, int mapHeight)
//{
//	auto properties = GetObjectProperties(item);
//	if (properties.size() == 0)
//		return nullptr;
//
//	auto width = 2 * stoi(properties["width"]);
//	auto height = 2 * stoi(properties["height"]);
//
//	auto x = 2 * stoi(properties["x"]);
//	auto y = mapHeight - 2 * stoi(properties["y"]) - height;
//
//
//	auto back = new Back(x, y, width, height);
//	back->setNextStage((eID)stoi(properties["next"]));
//	return back;
//}
//
BaseObject* GetRevive(xml_node item, int mapHeight)
{
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;

	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;


	auto end = new Revive(x, y, width, height);
	return end;
}

BaseObject* GetStart(xml_node item, int mapHeight)
{
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;

	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;


	auto start = new Start(x, y, width, height);
	start->setStatus((eStatus) stoi(properties["status"]));

	return start;
}

//BaseObject* GetFrogRange(xml_node item, int mapHeight)
//{
//	auto properties = GetObjectProperties(item);
//	if (properties.size() == 0)
//		return nullptr;
//
//	auto width = 2 * stoi(properties["width"]);
//	auto height = 2 * stoi(properties["height"]);
//
//	auto x = 2 * stoi(properties["x"]);
//	auto y = mapHeight - 2 * stoi(properties["y"]) - height;
//
//
//	auto range = new FrogRange(x, y, width, height);
//	range->init();
//	return range;
//}
//
//BaseObject* GetBirdRange(xml_node item, int mapHeight)
//{
//	auto properties = GetObjectProperties(item);
//	if (properties.size() == 0)
//		return nullptr;
//
//	auto width = 2 * stoi(properties["width"]);
//	auto height = 2 * stoi(properties["height"]);
//
//	auto x = 2 * stoi(properties["x"]);
//	auto y = mapHeight - 2 * stoi(properties["y"]) - height;
//
//
//	auto birdRange = new BirdRange(x, y, width, height);
//	birdRange->init();
//	return birdRange;
//}
//
//BaseObject* GetDoor(xml_node item, int mapHeight)
//{
//	auto properties = GetObjectProperties(item);
//	if (properties.size() == 0)
//		return nullptr;
//	auto width = 2 * stoi(properties["width"]);
//	auto height = 2 * stoi(properties["height"]);
//
//	auto x = 2 * stoi(properties["x"]);
//	auto y = mapHeight - 2 * stoi(properties["y"]) - height;
//
//	x = x + width / 2;
//	y = y + height / 2;
//	auto door = new Door(x, y);
//	door->init();
//	return door;
//}
//
BaseObject* GetWall(xml_node item, int mapHeight)
{
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;

	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;


	auto wall = new Wall(x, y, width, height);
	wall->init();
	return wall;
}

BaseObject* GetMonkeyWall(xml_node item, int mapHeight)
{
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;

	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;


	auto monkeyWall = new MonkeyWall(x, y, width, height);
	monkeyWall->init();
	return monkeyWall;
}

BaseObject* GetFishmanWall(xml_node item, int mapHeight)
{
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;

	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;


	auto fishmanWall = new FishmanWall(x, y, width, height);
	fishmanWall->init();
	return fishmanWall;
}

BaseObject* GetBreakWall(xml_node item, int mapHeight)
{
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;
	BaseObject* breakWall = nullptr;
	if (properties["item"] != "")
		breakWall = new BreakWall(x, y, (eID)stoi(properties["item"]));
	else
		breakWall = new BreakWall(x, y);
	breakWall->init();
	return breakWall;
}

BaseObject* GetCandle(xml_node item, int mapHeight)
{
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;
	BaseObject* candle = nullptr;
	if (properties["Item"] != "")
		candle = new Candle(x, y, (eID) stoi(properties["Item"]));
	else
		candle = new Candle(x, y);
	candle->init();
	return candle;
}
BaseObject* GetFloatingWall(xml_node item, int mapHeight)
{
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;

	auto ActiveBoundX = 2 * stoi(properties["ActiveBoundX"]);
	bool Reversed = false;
	auto value = properties["Reversed"];
	if (value == "true")
		Reversed = true;
	bool isQuadtreeNode = true;
	auto quadtreeVal = properties["isQuadtreeNode"];
	if (quadtreeVal == "false")
		isQuadtreeNode = false;

	ActiveBoundX = ActiveBoundX + width / 2;

	auto floatingWall = new FloatingWall(x, y, ActiveBoundX, Reversed, isQuadtreeNode);
	floatingWall->init();
	return floatingWall;
}

BaseObject* GetBat(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	bool isQuadtreeNode = true;
	auto quadtreeVal = properties["isQuadtreeNode"];
	if (quadtreeVal == "false")
		isQuadtreeNode = false;

	x = x + width / 2;
	y = y + height / 2;

	auto bat = new Bat(x, y, isQuadtreeNode);
	bat->init();
	return bat;
}

BaseObject* GetFishman(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;

	bool isLeft = false;
	auto value = properties["isLeft"];
	if (value == "true")
		isLeft = true;

	auto fishman = new Fishman(x, y, isLeft);
	fishman->init();
	return fishman;
}

BaseObject* GetYellowBag(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;


	auto yelloBag = new YellowBag(x, y);
	yelloBag->init();
	return yelloBag;
}
BaseObject* GetRedBag(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;


	auto redBag = new RedBag(x, y);
	redBag->init();
	return redBag;
}
BaseObject* GetHeart(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;


	auto heart = new Heart(x, y);
	heart->init();
	return heart;
}
BaseObject* GetBigHeart(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;


	auto bigHeart = new BigHeart(x, y);
	bigHeart->init();
	return bigHeart;
}

BaseObject* GetHolyWater(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;


	auto holyWater = new HolyWater(x, y);
	holyWater->init();
	return holyWater;
}

BaseObject* GetDagger(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;


	auto dagger = new Dagger(x, y);
	dagger->init();
	return dagger;
}

BaseObject* GetFood(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;


	auto food = new Food(x, y);
	food->init();
	return food;
}

BaseObject* GetFrankenstein(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);
	auto checkpoint =2 * stoi(properties["checkpoint"]);
	auto ballPos = 2 * stoi(properties["ballPosition"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;

	bool isQuadtreeNode = true;
	auto quadtreeVal = properties["isQuadtreeNode"];
	if (quadtreeVal == "false")
		isQuadtreeNode = false;


	auto frankenstein = new Frankenstein(x, y, checkpoint, ballPos, isQuadtreeNode);
	frankenstein->init();
	return frankenstein;
}

BaseObject* GetDinosaur(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;


	auto dinosaur = new Dinosaur(x, y);
	dinosaur->init();
	return dinosaur;
}

BaseObject* GetMonkey(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto quadtree = properties["isQuadtreeNode"];
	bool _isQuadtreeNode = true;
	if (quadtree == "false")
	{
		_isQuadtreeNode = false;
	}
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;


	auto monkey = new Monkey(x, y, 200, _isQuadtreeNode);
	monkey->init();
	return monkey;
}



BaseObject* GetStopTime(xml_node item, int mapHeight) {
	auto properties = GetObjectProperties(item);
	if (properties.size() == 0)
		return nullptr;
	auto width = 2 * stoi(properties["width"]);
	auto height = 2 * stoi(properties["height"]);

	auto x = 2 * stoi(properties["x"]);
	auto y = mapHeight - 2 * stoi(properties["y"]) - height;

	x = x + width / 2;
	y = y + height / 2;


	auto stopTime = new StopTime(x, y);
	stopTime->init();
	return stopTime;
}

BaseObject* GetObjectByType(xml_node item, eID type, int mapHeight)
{
	switch (type)
	{
		case WALL:
			return GetWall(item, mapHeight);
		case FISHMAN_WALL:
			return GetFishmanWall(item, mapHeight);
		case WATER:
			return GetWater(item, mapHeight);
		case FLOATING_WALL:
			return GetFloatingWall(item, mapHeight);
		case MONKEY_WALL:
			return GetMonkeyWall(item, mapHeight);
		case START:
			return GetStart(item, mapHeight);
		case END:
			return GetEnd(item, mapHeight);
		case REVIVE:
			return GetRevive(item, mapHeight);
		case CANDLE:
			return GetCandle(item, mapHeight);
		case BAT:
			return GetBat(item, mapHeight);
		case FISHMAN:
			return GetFishman(item, mapHeight);
		case FRANKENSTEIN:
			return GetFrankenstein(item, mapHeight);
		case DINOSAUR:
			return GetDinosaur(item, mapHeight);
		case MONKEY:
			return GetMonkey(item, mapHeight);
		case BREAK_WALL:
			return GetBreakWall(item, mapHeight);
		default:
			return nullptr;
	}
}

list<BaseObject*>* GetListObjectFromFile(const string path)
{
	xml_document doc;
	list<BaseObject*>* listObj = new list<BaseObject*>();

	auto result = doc.load_file(path.data(), pugi::parse_default | pugi::parse_pi);
	if (result == false)
	{
		return listObj;
	}
	auto map = doc.child("map");
	if (map == NULL)
		return listObj;

	auto mapHeight = 2 * map.attribute("tileheight").as_int() * map.attribute("height").as_int();

	xml_node objectGroup = map.child("objectgroup");
	if (objectGroup == NULL)
		return listObj;

	auto objects = objectGroup.children();

	for (auto obj : objects)
	{
		int id = obj.attribute("id").as_int();
		auto type = obj.attribute("type").as_int();

		eID enumID;
		try
		{
			enumID = (eID)type;
		}
		catch (exception e)
		{
			continue;
		}

		auto baseObj = GetObjectByType(obj, enumID, mapHeight);

		if (baseObj != nullptr)
		{
			listObj->push_back(baseObj);
		}
	}
	return listObj;
}


