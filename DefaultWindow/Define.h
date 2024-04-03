#pragma once

extern	HWND		g_hWnd;

#define			WINCX		800
#define			WINCY		600

#define			PURE		= 0

#define			OBJ_NOEVENT		0
#define			OBJ_DEAD		1

#define			PI			3.141592f

#define			VK_MAX		0xff

#define			TILECX		64
#define			TILECY		64

#define			TILEX		64
#define			TILEY		26

#define SINGLE(type)\
private:\
	type();\
	~type();\
public:\
	static type* CreateSingleTonInst()\
	{\
		static type mgr;\
		return &mgr;\
	}\

enum OBJID { OBJ_BOX, OBJ_ROPE, OBJ_CHEST, OBJ_HOLDOBJ, OBJ_OBSTACLE, OBJ_ITEM, OBJ_MONSTER, OBJ_PLAYER, OBJ_LAVA, OBJ_BOMB, OBJ_EFFECT, OBJ_UI, OBJ_END };
enum RENDERID { RENDER_BACKGROUND, RENDER_GAMEOBJECT, RENDER_HOLDOBJ, RENDER_TILE, RENDER_GHOST, RENDER_UI, RENDER_EFFECT, RENDER_END };

enum SCENEID { SC_LOGO, SC_MENU, SC_EDIT, SC_STAGE, SC_STAGE2, SC_STAGE3, SC_END };

enum CHANNELID
{
	SOUND_EFFECT,
	SOUND_BGM,
	MAXCHANNEL
};

enum class OBJECT_TYPE	//이거나 OBJID나 머가다른지 모르곘네
{
	NONE,
	ROPE,
	BOX,
	OBSTACLE,
	CHEST,
	BOMB,
	PLAYER,
	MONSTER,
	ITEM,
	HOLDOBJ,
	LAVA,
	EFFECT,
	UI,
	END
};

enum DIRECTION
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	LU,
	RU,
	DIR_END
};

typedef struct tagInfo
{
	float		fX;
	float		fY;

	float		fCX;
	float		fCY;

}INFO;

typedef struct tagPoint
{
	float	fX;
	float	fY;

	tagPoint() { ZeroMemory(this, sizeof(tagPoint)); }
	tagPoint(float _fX, float _fY) : fX(_fX), fY(_fY) {}

}LINEPOINT;

typedef struct tagLine
{
	LINEPOINT		tLPoint;
	LINEPOINT		tRPoint;
	tagLine() { ZeroMemory(this, sizeof(tagLine)); }
	tagLine(LINEPOINT& _tLPoint, LINEPOINT& _tRPoint)
		: tLPoint(_tLPoint), tRPoint(_tRPoint){}

}LINE;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iMotion;
	int		iFrameMax;
	bool	bRoop;
	DWORD	dwSpeed;
	DWORD	dwTime;
	int		iRepeat = 1;
}FRAME;



template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

template<typename T>
void Safe_Delete_Array(T& Temp)
{
	if (Temp)
	{
		delete[] Temp;
		Temp = nullptr;
	}
}

struct DeleteObj
{
	template<typename T>
	void		operator()(T& Obj)
	{
		if (Obj)
		{
			delete Obj;
			Obj = nullptr;
		}
	}
};

struct DeleteMap
{
	template<typename T>
	void		operator()(T& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	}
};


class CStringCmp
{

public:
	CStringCmp(const TCHAR* pString) : m_pString(pString) {}

public:
	template<typename T>
	bool operator()(T& MyPair)
	{
		return !lstrcmp(m_pString, MyPair.first);
	}

private:
	const	TCHAR*		m_pString;
};