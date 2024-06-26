#pragma once

#include "Obj.h"

class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static bool Check_Rect(CObj* pDst, CObj* pSrc, float* pX, float* pY);
	static bool Check_RectChest(CObj* pDst, CObj* pSrc);

	static void	Collision_Rect(CObj* Dst, CObj* Src);

	static void Collision_RectMon(CObj* Dst, CObj* Src);
	static void Collision_RectHoldMon(list<CObj*> _Dst, list<CObj*> _Src);
	static void	Collision_RectEx(CObj* _Dst, CObj* _Src);
	static void Collision_RectChest(CObj* Dst, CObj* Src);

	static void	Collision_Sphere(list<CObj*> _Dst, list<CObj*> _Src);
	static bool Check_Sphere(CObj* pDst, CObj* pSrc);
};

