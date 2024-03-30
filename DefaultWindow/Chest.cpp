#include "stdafx.h"
#include "Chest.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "Item.h"
#include "Player.h"

CChest::CChest()
{
	m_eMyObjType = OBJECT_TYPE::CHEST;
}

CChest::~CChest()
{
	Release();
}

void CChest::Initialize()
{
	m_tInfo = { 0, 0, TILECX, TILECY };
	m_pFrameKey = L"Chest";
	m_eRender = RENDER_GAMEOBJECT;
}

int CChest::Update()
{
	if (m_bDead)
		return OBJ_DEAD;


	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CChest::Late_Update()
{
	INFO _playerInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

	if (((m_tInfo.fX - m_tInfo.fCX - 10 <= _playerInfo.fX && _playerInfo.fX <= m_tInfo.fX)
		|| (m_tInfo.fX < _playerInfo.fX && _playerInfo.fX <= m_tInfo.fX + m_tInfo.fCX + 10))
		&& m_tInfo.fY - m_tInfo.fCY / 2 <= _playerInfo.fY && _playerInfo.fY <= m_tInfo.fY + m_tInfo.fCY / 2
		&& dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetState() == CPlayer::ATTACK)
	{
		m_bDead = true;
	}
}

void CChest::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, (m_eItemID / 2) * (int)m_tInfo.fCX, 0, TILECX, TILECY, RGB(55, 55, 55));
}

void CChest::Release()
{
	if (m_eItemID == CItem::ITEM_GEM)	// 보석일 경우 1~3개의 보석을 떨어뜨린다.
	{
		int _rand = 1 + rand() % 3;
		for (int i = 0; i < _rand; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CItemFactory::Create(m_tInfo.fX + 8.f * i, m_tInfo.fY - 10.f, m_eItemID));
		}
	}
	else
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CItemFactory::Create(m_tInfo.fX, m_tInfo.fY - 10.f, m_eItemID));
}
