#include "stdafx.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Player.h"

CObjMgr*		CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}


CObj * CObjMgr::Get_Target(OBJID eID, CObj * pObj)
{
	if(m_ObjList[eID].empty())
		return nullptr;

	CObj*	pTarget = nullptr;

	float	fDistance(0.f);

	for (auto& iter : m_ObjList[eID])
	{
		if(iter->Get_Dead())
			continue;

		float	fWidth	  = pObj->Get_Info().fX - iter->Get_Info().fX;
		float	fHeight	  = pObj->Get_Info().fY - iter->Get_Info().fY;
		float	fDiagonal = sqrt(fWidth * fWidth + fHeight * fHeight);

		if ((!pTarget) || (fDistance > fDiagonal))
		{
			pTarget = iter;
			fDistance = fDiagonal;
		}
	}

	return pTarget;
}

void CObjMgr::Add_Object(OBJID eID, CObj * pObj)
{
	if (OBJ_END <= eID || nullptr == pObj || eID == OBJ_PLAYER && m_ObjList[eID].empty() == false)
		return;

	m_ObjList[eID].push_back(pObj);
}

void CObjMgr::Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); )
		{
			int		iResult = (*iter)->Update();

			if (OBJ_DEAD == iResult)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CObjMgr::Late_Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
		{
			iter->Late_Update();

			if (m_ObjList[i].empty())
				break;

			RENDERID eID = iter->Get_GroupID();
			m_RenderList[eID].push_back(iter);
		}
	}

	for (auto iter : m_ObjList[OBJ_MONSTER])
	{
		CCollisionMgr::Collision_RectMon(iter, m_ObjList[OBJ_PLAYER].front());
	}
	for (auto iter : m_ObjList[OBJ_ITEM])
	{
		CCollisionMgr::Collision_RectChest(iter, m_ObjList[OBJ_PLAYER].front());
	}
	for (auto iter : m_ObjList[OBJ_BOX])
	{
		CCollisionMgr::Collision_RectEx(iter, m_ObjList[OBJ_PLAYER].front());
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetAttatchedBox() == true)
			break;
	}
	for (auto iter : m_ObjList[OBJ_HOLDOBJ])
	{
		CCollisionMgr::Collision_Rect(iter, m_ObjList[OBJ_PLAYER].front());
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetCanHold() == true)
			break;
	}
	if(m_ObjList[OBJ_MONSTER].empty() == false && m_ObjList[OBJ_HOLDOBJ].empty() == false)
	CCollisionMgr::Collision_RectHoldMon(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_HOLDOBJ]);
}

void CObjMgr::Render(HDC hDC)
{

	for (size_t i = 0; i < RENDER_END; ++i)
	{
		if (i == RENDER_UI || i == RENDER_GHOST)
			continue;

		for (auto& iter : m_RenderList[i])
			iter->Render(hDC);

		m_RenderList[i].clear();
	}
}

void CObjMgr::RenderGhost(HDC hDC)
{
	for (auto& iter : m_RenderList[RENDER_GHOST])
		iter->Render(hDC);

	m_RenderList[RENDER_GHOST].clear();
}

void CObjMgr::RenderUI(HDC hDC)
{

	for (auto& iter : m_RenderList[RENDER_UI])
		iter->Render(hDC);

	m_RenderList[RENDER_UI].clear();
}


void CObjMgr::Release()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
}

void CObjMgr::Delete_ID(OBJID eID)
{
	for (auto& pObj : m_ObjList[eID])
		Safe_Delete(pObj);

	m_ObjList[eID].clear();
}

void CObjMgr::ReleaseWithoutPlayer()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		if (i == OBJ_PLAYER)
			continue;

		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
}
