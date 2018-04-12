﻿#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800 
#endif

#pragma warning(disable: 4018)
#pragma warning(disable: 4066)
#pragma warning(disable: 4244)
#pragma warning(disable: 4305)
#pragma warning(disable: 4715)
#pragma warning(disable: 4800)

#ifndef __GAMEUIT_FRAMEWORK__
#define __GAMEUIT_FRAMEWORK__

#define _USE_MATH_DEFINES

#include <d3d9.h>		
#include <d3dx9.h>	
#include <dinput.h>		
#include <dsound.h>		
#include <windows.h>
#include <exception>
#include <math.h>
#include <string>
#include <map>
#include <vector>
#include "utils.h"
using namespace std;

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 450
#define SCALE_FACTOR 1.0f

#define COLOR_WHITE D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)				// màu trắnng
#define COLOR_KEY D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f)				

#define ITEM_DESTROY_TIME 3000

#define MAX_OBJECTS 4
#define MAX_LEVELS 32

enum eID
{
	PLAYER = 0,
	MAIN_MENU = 1,
	FONTFULL = 2,

	INFO = 4,
	HEART_ICON = 5,
	HIT_POINT_ICON = 6,
	BORDER = 7,

	MAP_STAGE_10 = 10,
	MAP_STAGE_12 = 12,

	ROPE = 30,
	EFFECT = 31,
	HEAL = 32,

	WALL = 40,
	FLOATING_WALL = 43,
	WATER = 44,
	FLOATING_WALL_REVERSED = 45,

	START = 50,
	END = 51,
	REVIVE = 53,
	DOOR = 54,

	CANDLE = 61,

	OBJECTS = 79,
	ITEM = 80,

	STOPTIME = 89,

};

enum eStatus
{
	NORMAL = 0,
	MOVING_LEFT = (1 << 0),
	MOVING_RIGHT = (1 << 1),
	JUMPING = (1 << 2),
	RUNNING = (1 << 3),
	DIE = (1 << 4),
	SIT_DOWN = (1 << 5),
	FALLING = (1 << 6),

	DESTROY = (1 << 7),

	ATTACKING = (1 << 8),

	MOVING_UP = (1 << 9),
	MOVING_DOWN = (1 << 10),

	STAND_UP = (1 << 11),
	STAND_DOWN = (1 << 12),

	BEING_HIT = (1 << 13),
};

enum eDirection
{
	NONE = 0,
	TOP = 1,
	BOTTOM = 2,
	LEFT = 4,
	RIGHT = 8,
	TOP_BOTTOM = (TOP | BOTTOM),
	ALL = (TOP | BOTTOM | LEFT | RIGHT),
};

enum eSoundId
{
	INTRO_SCENE,
	PLAY_SCENE,
	OVER_SCENE,

	OPEN_DOOR,
	WIN_LEVEL,

	HIT_SOUND,
	HIT_DINOSAUR,
	HIT_WALL,
	DIE_SOUND,
	FALLING_WALL,

	AXE_SOUND,
	BOOMERANG_SOUND,
	DAGGER_SOUND,

	CHANGE_STAGE,

	BOSS_SOUND,

	TIME_OUT,
	
	COLISION_SOUND,

	GET_MONEY,
	GET_HEART,
	GET_POTION,
	GET_CROSS,
	GET_ITEM,

	HIDDEN_ITEM_APPEAR,
};

typedef D3DXVECTOR3 GVector3;
typedef D3DXVECTOR2 GVector2;
#define VECTOR2ZERO GVector2(0.0f, 0.0f)
#define VECTOR2ONE  GVector2(1.0f, 1.0f)

#define EVENT_RECEIVER [event_receiver(native)]

#ifndef SAFE_DELETE

#define SAFE_DELETE(p) \
if(p) \
{\
	delete (p); \
	p = nullptr; \
} \

#endif // !SAFE_DELETE


#endif // !__GAMEUIT_FRAMEWORK__
