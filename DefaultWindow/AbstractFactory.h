#pragma once

#include "Obj.h"
#include "LineMgr.h"
#include "Line.h"
#include "Box.h"
#include "Rope.h"
#include "Bomb.h"

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