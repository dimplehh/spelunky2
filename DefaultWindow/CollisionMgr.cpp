#include "stdafx.h"
#include "CollisionMgr.h"
#include "Player.h"
#include "KeyMgr.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src)
{
	RECT	rcRect{};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rcRect, &(Dst->Get_Rect()), &(Src->Get_Rect())))
			{
				//Dst->SetCollision(true);
				//Src->SetCollision(true);

				//if (OBJECT_TYPE::PLAYER == Dst->Get_MyObjType())
				//{
				//	if (OBJECT_TYPE::MONSTER == Src->Get_MyObjType())
				//	{
				//		Dst->SetCollision(true);
				//		Src->SetCollision(true);
				//		dynamic_cast<CPlayer*>(Dst)->SetAttachedBox(true);
				//	}
				//	/*else
				//		dynamic_cast<CPlayer*>(Dst)->SetAttachedBox(false);*/
				//}
				
			}
		}
	}
}


void CCollisionMgr::Collision_RectEx(CObj* Dst, CObj* Src)
{
	float	fX(0.f), fY(0.f);

	if (Check_Rect(Dst, Src, &fX, &fY))
	{
		if (fX <= fY)		// 谅快 面倒
		{
			// 谅 面倒
			if (Dst->Get_Info().fX < Src->Get_Info().fX)
			{
				Dst->Set_PosX(-fX);
			}
			// 快 面倒
			else
			{
				Dst->Set_PosX(fX);
			}
		}
		Dst->SetCollision(true);
		Src->SetCollision(true);
		if (OBJECT_TYPE::PLAYER == Src->Get_MyObjType())
		{
			if (OBJECT_TYPE::BOX == Dst->Get_MyObjType())
			{
				Dst->SetCollision(true);
				Src->SetCollision(true);
				if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD || 
					CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD)
					dynamic_cast<CPlayer*>(Src)->SetAttachedBox(true);
			}
		}
	}
	else
		dynamic_cast<CPlayer*>(Src)->SetAttachedBox(false);
}

bool CCollisionMgr::Check_Rect(CObj * pDst, CObj * pSrc, float * pX, float * pY)
{
	float srcRealX;

	if(pDst->Get_Info().fX < pSrc->Get_Info().fX)
		srcRealX = pSrc->Get_Info().fX + 15;
	else
		srcRealX = pSrc->Get_Info().fX - 15;

	float	fWidth  = abs(pDst->Get_Info().fX - srcRealX);
	float	fHeight = abs(pDst->Get_Info().fY - pSrc->Get_Info().fY);

	float	fRadiusX = (pDst->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;
	float	fRadiusY = (pDst->Get_Info().fCY + pSrc->Get_Info().fCY) * 0.5f;

	if ((fRadiusX >= fWidth) && (fRadiusY >= fHeight))
	{
		*pX = fRadiusX - fWidth;
		*pY = fRadiusY - fHeight;

		return true;
	}

	return false;
}

void CCollisionMgr::Collision_Sphere(list<CObj*> _Dst, list<CObj*> _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dst, Src))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

bool CCollisionMgr::Check_Sphere(CObj * pDst, CObj * pSrc)
{
	float	fWidth  = abs(pDst->Get_Info().fX - pSrc->Get_Info().fX);
	float	fHeight	= abs(pDst->Get_Info().fY - pSrc->Get_Info().fY);

	float	fDiagonal = sqrt(fWidth * fWidth + fHeight  * fHeight);

	float	fRadius = (pDst->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;
		
	return fRadius >= fDiagonal;
}
