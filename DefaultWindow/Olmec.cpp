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

COlmec::COlmec() : m_ePreState(ST_END), m_eCurState(IDLE), m_dwTime(GetTickCount()), m_pVecTile(nullptr), m_headLine(nullptr)
{
}

COlmec::~COlmec()
{
}

void COlmec::Initialize()
{
	m_tInfo = { 0.f, 0.f, TILECX * 3.f , TILECY * 3.f };
	m_fSpeed = 1.5f;
	m_iHp = 100000;
	m_iAttackPower = 100000;
	m_pFrameKey = L"Olmec";
	m_pVecTile = CTileMgr::Get_Instance()->Get_VecTile();

	m_tFrame = { 0, 0, 0, 5, true, 60, GetTickCount(), 0 }; //일단 애니메이션 현재는 안쓰긴 하지만 차후에 쓸듯..
	m_eRender = RENDER_GAMEOBJECT;
}

int COlmec::Update()
{
	if (m_bDead == true)
		return OBJ_DEAD;

	if (m_tInfo.fY > TILECY * TILEY - TILECY * 4) //용암에 빠졌을 경우 모든 동작을 멈춤
		return OBJ_NOEVENT;
	
	if (m_iPhase == 0)
	{
		// 플레이어가 울맥 위치까지 오는것 기다렸다가 카메라에 울맥 들어오면 컷신 나오면서 컷신 끝나면 페이즈 1로 변함

		if (m_tInfo.fX - TILECX * 5.f <= CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX
			&& CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX < m_tInfo.fX + TILECX * 5.f)
		{
			// 컷신 실행 함수 넣고 컷신이 끝날 경우 m_iPhase = 1;
			m_iPhase = 1;
		}
	}
	else if (m_iPhase == 1)
	{
		Idle();
		Rise();
		Smash();
	}
	else if (m_iPhase == 2)  // 두번째 구간 진입 시
	{
		//플레이어 화면 안으로 들어오는것 기다린 후 들어오면 공격패턴 시작
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void COlmec::Late_Update()
{
	if (m_bCanSmash == true)
		Gravity();

	m_headLine->Set_Info(LINE(LINEPOINT{ m_tInfo.fX - TILECX * 1.6f , m_tInfo.fY - TILECY * 1.2f}, LINEPOINT{ m_tInfo.fX + TILECX * 1.6f, m_tInfo.fY - TILECY * 1.2f}));
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
	if (CLineMgr::Get_Instance()->Collision_Olmec_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
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
		if (m_tInfo.fY >= m_fPreY - TILECY * 2.5f)
		{
			m_tInfo.fY -= 5.f;
			if (abs(m_fPrePlayerX - m_tInfo.fX) >= 5)
			{
				if (m_fPrePlayerX < m_tInfo.fX)
					m_tInfo.fX -= 4.f;
				else
					m_tInfo.fX += 4.f;
			}
		}
		else
		{
			m_bCanSmash = true;
			m_bCheckOneTime = true;
		}
	}
}

void COlmec::Smash()
{
	if (m_bCanSmash)
	{
		m_bCanRise = false;
		if (CLineMgr::Get_Instance()->Collision_Olmec_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
		{
			if (m_bCheckOneTime == true)
			{
				Break();
				m_bCheckOneTime = false;
			}
			if (CLineMgr::Get_Instance()->Collision_Olmec_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
			{
				m_bCanSmash = false;
			}
		}
	}
}

void COlmec::Break() //타일 깨뜨리는 함수
{
	int index = CTileMgr::Get_Instance()->Get_Tile_Idx(m_tInfo.fX, m_tInfo.fY);

	if (0 > index || (size_t)index >= (*m_pVecTile).size())
		return;

	for (int _add = -1; _add <= 1; _add++)
	{
		SetBrokenTile(index + _add + TILEX * 2);
	}
/*	if (GetUpNoBrokenTile(index) == true)
	{
		for (int _add = -1; _add <= 1; _add++)
		{
			SetBrokenTile(index + _add + TILEX);
		}
	}
	else if(0 <= index && (size_t)index < (*m_pVecTile).size())
	{
		for (int _add = -1; _add <= 1; _add++)
		{
			SetBrokenTile(index + _add + TILEX * 2);
	}
	}*/

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

bool COlmec::GetUpNoBrokenTile(int index)
{
	if (0 > index || (size_t)index >= (*m_pVecTile).size())
		return false;

	for (int _add = -1; _add <= 1; _add++)
	{
		if (!((*m_pVecTile)[index + _add + TILEX]->Get_Option() == 0 && (*m_pVecTile)[index + _add + TILEX]->Get_DrawID() == 0))
		{
			return true;
		}
	}
	return false;
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
