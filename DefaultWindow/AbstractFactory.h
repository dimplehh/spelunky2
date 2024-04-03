#pragma once

#include "Obj.h"
#include "LineMgr.h"
#include "Line.h"
#include "Box.h"
#include "Obstacle.h"
#include "Rope.h"
#include "Bomb.h"
#include "Chest.h"
#include "UIIcon.h"
#include "Item.h"
#include "Monster.h"
#include "Snake.h"
#include "Ghost.h"
#include "Frog.h"
#include "Olmec.h"
#include "HoldObj.h"
#include "ObjMgr.h"

template<typename T>
class CAbstractFactory
{
public:
	static CObj*		Create()
	{
		CObj*		pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj*		Create(float _fX, float _fY)
	{
		CObj*		pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		if (pObj->Get_MyObjType() == OBJECT_TYPE::BOMB)
			dynamic_cast<CBomb*>(pObj)->Set_PreY(_fY);
		return pObj;
	}

	static CObj*		Create(float _fX, float _fY, float _fAngle)
	{
		CObj*		pObj = new T;
		pObj->Initialize();
		
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Angle(_fAngle);
		return pObj;
	}


public:
	CAbstractFactory(){}
	~CAbstractFactory(){}
};

class CBombFactory
{
public:
	static CObj* Create(float _fX, float _fY, bool _flip)
	{
		CObj* pObj = new CBomb;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		dynamic_cast<CBomb*>(pObj)->Set_PreY(_fY);
		dynamic_cast<CBomb*>(pObj)->SetFlip(_flip);

		return pObj;
	}
};

class CLineFactory
{
public:
	static CLine* Create(LINEPOINT lPoint, LINEPOINT rPoint, CLine::LINETYPE lineType)
	{
		CLine* pLine = new CLine(lPoint, rPoint);
		dynamic_cast<CLine*>(pLine)->Set_LineType(lineType);
		return pLine;
	}
};

class CBoxFactory
{
public:
	static CBox* Create(float _fX, float _fY)
	{
		CBox* pObj = new CBox;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);

		CLineMgr::Get_Instance()->Set_Box_Line(_fX, _fY);
		pObj->SetBoxLine(CLineMgr::Get_Instance()->Get_Box_Line()->back());
		return pObj;
	}
};

class CRopeFactory
{
public:
	static CRope* Create(float _fX, float _fY)
	{
		CRope* pObj = new CRope;
		pObj->Initialize();
		int height = CLineMgr::Get_Instance()->Check_Rope_Attach_Ceiling(_fX, _fY);	//천장위치 체킹

		if (height >= 6 * TILECY)
			height = 6 * TILECY;

		pObj->Set_Pos(_fX, _fY - height);
		dynamic_cast<CRope*>(pObj)->SetRopeSize(height / TILECY);
		CLineMgr::Get_Instance()->MakeRopeLine(LINEPOINT{ _fX, _fY + TILECY / 2 },LINEPOINT{_fX, _fY - height });
		return pObj;
	}
};

class CUIFactory
{
public:
	static CObj* Create(float _fX, float _fY, CUIIcon::UIID _uiId, int _num = 0, int _iSize = 16, float _fTextX = 0, float _fTextY = 0)
	{
		CObj* pObj = new CUIIcon;
		
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		
		dynamic_cast<CUIIcon*>(pObj)->SetUIID(_uiId);
		dynamic_cast<CUIIcon*>(pObj)->Set_Size(_iSize);
		dynamic_cast<CUIIcon*>(pObj)->Set_Num(_num);
		dynamic_cast<CUIIcon*>(pObj)->SetTextXY(_fTextX, _fTextY);

		return pObj;
	}
};

class CItemFactory
{
public:
	static CObj* Create(float _fX, float _fY, CItem::ITEMID _ItemId)
	{
		CObj* pObj = new CItem;

		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);

		dynamic_cast<CItem*>(pObj)->Set_ItemID(_ItemId);

		int num = 0;
		if (_ItemId == CItem::ITEM_BOMB || _ItemId == CItem::ITEM_ROPE)
			num = 3;
		else if (_ItemId == CItem::ITEM_GEM)
			num = 500;
		else if (_ItemId == CItem::ITEM_GOLD)
			num = 1000;

		dynamic_cast<CItem*>(pObj)->Set_Num(num);

		return pObj;
	}
};

class CChestFactory
{
public:
	static CObj* Create(float _fX, float _fY, CItem::ITEMID _itemID)
	{
		CObj* pObj = new CChest;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		dynamic_cast<CChest*>(pObj)->SetChestItemID(_itemID);
		return pObj;
	}

	static CObj* CreateHoldChest(float _fX, float _fY, CHoldObj::HOLDOBJID _holdObjID)
	{
		CObj* pObj = new CChest;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		dynamic_cast<CChest*>(pObj)->SetHoldObjID(_holdObjID);
		return pObj;
	}
};

class CHoldObjFactory
{
public:
	static CObj* Create(float _fX, float _fY, CHoldObj::HOLDOBJID _holdObjID)
	{
		CObj* pObj = new CHoldObj;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		dynamic_cast<CHoldObj*>(pObj)->Set_HoldObjID(_holdObjID);

		int _idx = CObjMgr::Get_Instance()->GetHoldObjSize();
		dynamic_cast<CHoldObj*>(pObj)->SetIdx(_idx);
		
		return pObj;
	}
};

class CMonsterFactory
{
public:
	static CObj* Create(float _fX, float _fY, CMonster::MONSTERID _monsterID)
	{
		CObj* pObj = nullptr;
		if (_monsterID == CMonster::SNAKE)
			pObj = new CSnake;
		else if (_monsterID == CMonster::GHOST)
			pObj = new CGhost;
		else if (_monsterID == CMonster::FROG)
			pObj = new CFrog;
		else if (_monsterID == CMonster::OLMEC)
		{
			pObj = new COlmec;
			CLineMgr::Get_Instance()->Set_Head_Line(_fX, _fY);
			dynamic_cast<COlmec*>(pObj)->SetHeadLine(CLineMgr::Get_Instance()->Get_Head_Line()->back());
		}

		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		dynamic_cast<CMonster*>(pObj)->SetMonsterID(_monsterID);
		return pObj;
	}
};