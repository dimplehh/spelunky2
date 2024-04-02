#include "stdafx.h"
#include "Olmec.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "UIMgr.h"
#include "Obj.h"
#include "TileMgr.h"
#include "Player.h"

COlmec::COlmec() : m_ePreState(ST_END), m_eCurState(IDLE), m_dwTime(GetTickCount())
{
}

COlmec::~COlmec()
{
}

void COlmec::Initialize()
{
	m_tInfo = { 0.f, 0.f, TILECX * 3.f , TILECY * 3.f };
	m_fSpeed = 1.5f;
	//m_fPower = 40.f;
	m_iHp = 100000;
	m_iAttackPower = 100000;
	//m_iMoveX = 6.f;
	m_pFrameKey = L"Olmec";
	m_pVecTile = CTileMgr::Get_Instance()->Get_VecTile();

	m_tFrame = { 0, 0, 0, 5, true, 60, GetTickCount(), 0 }; //일단 애니메이션 현재는 안쓰긴 하지만 차후에 쓸듯..
	m_eRender = RENDER_GAMEOBJECT;
}

int COlmec::Update()
{
	if (m_bDead == true)
		return OBJ_DEAD;

	Idle();
	Rise();
	Smash();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void COlmec::Late_Update()
{
	//if (m_bCanJump == false)
	//	Gravity();

	//Motion_Change();
	//__super::Move_Frame();

#ifdef _DEBUG

	if (m_dwTime + 500 < GetTickCount())
	{
		cout << m_fPreY << "/" << m_tInfo.fY << endl;
		m_dwTime = GetTickCount();
	}
#endif
}

void COlmec::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(81, 81, 81));
}

void COlmec::Release()
{
}

void COlmec::Idle()
{
	if (CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
	{
		m_fPreY = m_tInfo.fY;
		m_fPreX = m_tInfo.fX;
		m_fPrePlayerX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX;
	}

	if (CUIMgr::Get_Instance()->Get_Time() % m_iAttackCycle == 0)
	{
		m_bCanRise = true;
	}
}

void COlmec::Rise()
{
	if (m_bCanRise == true)
	{
		if (m_tInfo.fY > m_fPreY - TILECY * 2.f)
		{
			m_tInfo.fX += (m_fPrePlayerX - m_fPreX) / TILECX;
			m_tInfo.fY -= 7.f;
		}
		else
		{
			m_bCanSmash = true;
		}
	}
}

void COlmec::Smash() //여기서 타일도 깨뜨려야 함
{
	if (m_bCanSmash)
	{
		m_bCanRise = false;
		if (CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
		{
			Break();
			m_bCanSmash = false;
		}
		else
		{
			m_tInfo.fY += 7.f;
		}
	}
}

void COlmec::Break()
{
	int index = CTileMgr::Get_Instance()->Get_Tile_Idx(m_tInfo.fX, m_tInfo.fY);

	for (int _add = -1; _add <= 1; _add++)
	{
		SetBrokenTile(index + _add + TILEX * 2);
	}

	CLineMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->SetLine(); //라인 재세팅
}

void COlmec::SetBrokenTile(int index)
{
	if (0 > index || (size_t)index >= (*m_pVecTile).size())
		return;

	(*m_pVecTile)[index]->Set_FrameKey(L"Tile6");
	(*m_pVecTile)[index]->Set_Option(0);
	(*m_pVecTile)[index]->Set_DrawID(0);
}

void COlmec::Motion_Change() //차후 폭탄 발사 시 울맥 벌어질 때 필요
{
	//if (m_ePreState != m_eCurState)
	//{
	//	m_iRepeatCount = 0;
	//	switch (m_eCurState)
	//	{
	//	case COlmec::IDLE:		Set_Frame(0, 0, 0, true, 120, 0, 5);		break;
	//	}
	//	m_ePreState = m_eCurState;
	//}
}
