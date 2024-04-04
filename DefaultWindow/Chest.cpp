#include "stdafx.h"
#include "Chest.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "Item.h"
#include "Player.h"

CChest::CChest() : m_fTime(0.f), m_fPower(0.f)
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
	Gravity();

	CPlayer* player = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	INFO _playerInfo = player->Get_Info();

	if (((m_tInfo.fX - m_tInfo.fCX <= _playerInfo.fX && _playerInfo.fX <= m_tInfo.fX) && player->GetFlip() == false
	|| ((m_tInfo.fX < _playerInfo.fX && _playerInfo.fX <= m_tInfo.fX + m_tInfo.fCX) && player->GetFlip() == true))
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
	if (m_eItemID != CItem::ITEM_END)
	{
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hMemDC, (m_eItemID / 2) * (int)m_tInfo.fCX, 0, TILECX, TILECY, RGB(55, 55, 55));
	}
	else if (m_eHoldObjID != CHoldObj::HOLDOBJ_END)
	{
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hMemDC, 0, 0, TILECX, TILECY, RGB(55, 55, 55));
	}
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
	else if (m_eItemID != CItem::ITEM_END)
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CItemFactory::Create(m_tInfo.fX, m_tInfo.fY - 10.f, m_eItemID));
	else if (m_eHoldObjID != CHoldObj::HOLDOBJ_END)
		CObjMgr::Get_Instance()->Add_Object(OBJ_HOLDOBJ, CHoldObjFactory::Create(m_tInfo.fX, m_tInfo.fY - 10.f, m_eHoldObjID));
}

bool CChest::Gravity()
{
	if (!CLineMgr::Get_Instance()->Collision_Box_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, false))
	{						//if 플레이어가 충돌상태가 아니라면 + (점프상태가 아니라면)
		m_fTime += 0.2f;

		float fGravity = m_fPower * m_fTime - ((4.9f * m_fTime * m_fTime) * 0.5f); // 중력 적용 2번 검사하기에 중력값 절반

		if (fGravity < -5.f)	// 중력 속도의 최대치 설정
			fGravity = -5.f;

		if (fGravity > 5.f)	// 점프 속도의 최대치 설정
			fGravity = 5.f;

		m_tInfo.fY -= fGravity;	// y 값은 중력값만큼 변환
		return false;
	}
	else	// 플레이어가 충돌 중인경우
	{
		m_fPower = 0.f;			// 점프력 초기화
		return true;
	}
}