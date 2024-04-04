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
#include "UIMgr.h"
#include "AbstractFactory.h"
#include "Bomb.h"
#include "SoundMgr.h"
#include "EffectMgr.h"

extern float g_fVolume;

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
	m_pFrameKey = L"Olmec3";
	m_pVecTile = CTileMgr::Get_Instance()->Get_VecTile();

	m_tFrame = { 0, 0, 0, 5, true, 60, GetTickCount(), 0 }; 
	m_eRender = RENDER_GAMEOBJECT;
}

int COlmec::Update()
{
	if (m_bDead == true)
		return OBJ_DEAD;

	if (m_tInfo.fY > TILECY * TILEY - TILECY * 6) //용암에 빠졌을 경우 모든 동작을 멈춤
	{
		m_bInLava = true;
		return OBJ_NOEVENT;
	}

	switch (m_iPhase)
	{
	case 0:
		Phase0();
		break;
	case 1:
		Phase1();
		break;
	case 2:
		Phase2();
		break;
	default:
		break;
	}
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void COlmec::Late_Update()
{
	if (m_bCanSmash == true)
		Gravity();

	if(m_bInLava == false)
		m_headLine->Set_Info(LINE(LINEPOINT{ m_tInfo.fX - TILECX * 1.6f , m_tInfo.fY - TILECY * 0.7f}, LINEPOINT{ m_tInfo.fX + TILECX * 1.6f, m_tInfo.fY - TILECY * 0.7f}));

	Motion_Change();
	__super::Move_Frame();
}

void COlmec::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	if(m_bInLava == false)
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
			m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(81, 81, 81));
	else
	{
		m_headLine->Set_Info(LINE(LINEPOINT{ m_tInfo.fX - TILECX * 1.6f , m_tInfo.fY + TILECY * 6 - TILECY * 0.7f }, LINEPOINT{ m_tInfo.fX + TILECX * 1.6f, m_tInfo.fY + TILECY * 6 - TILECY * 0.7f }));
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + TILECY * 3 + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
			m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(81, 81, 81));
	}

}

void COlmec::Release()
{
}



void COlmec::Phase0()
{		// 플레이어가 울맥 위치까지 오는것 기다렸다가 카메라에 울맥 들어오면 컷신 나오면서 컷신 끝나면 페이즈 1로 변함

	if (m_tInfo.fX - TILECX * 5.f <= CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX
		&& CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX < m_tInfo.fX + TILECX * 5.f)
	{
		CScrollMgr::Get_Instance()->Set_ScrollXY(WINCX / 2 - m_tInfo.fX, WINCY / 2 - m_tInfo.fY);
		m_pFrameKey = L"Olmec3";
		m_eCurState = CUTSCENE;
	}
	if (m_eCurState == CUTSCENE && m_tFrame.iFrameStart == 4)
	{
		CScrollMgr::Get_Instance()->Set_ScrollXY(WINCX / 2 - CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX,
			WINCY / 2 - CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY);
		m_iPhase = 1;		
		m_pFrameKey = L"Olmec";
		m_eCurState = IDLE;
	}

	if (m_iPreFrameStart != m_tFrame.iFrameStart)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"StoneBreak.wav", SOUND_EFFECT, g_fVolume);
		if(m_tFrame.iFrameStart != 4)
			CEffectMgr::Get_Instance()->ActiveEffect(CEffect::EFFECT_STONE, m_tInfo.fX, m_tInfo.fY);
		m_iPreFrameStart = m_tFrame.iFrameStart;
	}
}

void COlmec::Phase1()
{
	if (m_tInfo.fY > TILECY * 12) //2 phase
	{
		m_iPhase = 2;
		return;
	}
	Idle();
	Rise();
	Smash();
}

void COlmec::Phase2() // 두번째 구간 진입 시 플레이어 화면 안으로 들어오는것 기다린 후 들어오면 공격패턴 시작
{
	if (m_bCanBroken == true)
	{
		Broken();
		return;
	}
	
	Idle2();
	Rise2();
	Move();
	Attack();
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
				CSoundMgr::Get_Instance()->PlaySound(L"Crush.wav", SOUND_EFFECT, g_fVolume);
				CEffectMgr::Get_Instance()->ActiveEffect(CEffect::EFFECT_STONE, m_tInfo.fX, m_tInfo.fY);
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
		SetBrokenTile(index + _add + TILEX);
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

void COlmec::Idle2()
{
	if (CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX < m_tInfo.fX - TILECX * 5.f ||
		m_tInfo.fX + TILECX * 5.f < CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX
		&& CLineMgr::Get_Instance()->Collision_Olmec_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
	{
		m_bCanSmash = false;
		m_bCanRise2 = true;
	}
}

void COlmec::Rise2()
{
	if (m_bCanRise2 == true)
	{
		if (m_bFirstChangeCheck == true)
		{
			m_tInfo.fCY = TILECY * 4;
			m_pFrameKey = L"Olmec2";
			m_fPreY2 = m_tInfo.fY;
			m_fPrePlayerX2 = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX;
			m_bFirstChangeCheck = false;
		}

		if (m_tInfo.fY >= m_fPreY2 - TILECY * 3)
		{
			m_tInfo.fY -= 5.f;
		}
		else
		{
			m_bCanMove = true;
		}
	}
}

void COlmec::Move()
{
	if (m_bCanMove == true)
	{
		m_eCurState = IDLE;
		if (abs(m_fPrePlayerX2 - m_tInfo.fX) >= 5)
		{
			if (m_fPrePlayerX2 < m_tInfo.fX)
				m_tInfo.fX -= 4.f;
			else
				m_tInfo.fX += 4.f;
		}
		if(CUIMgr::Get_Instance()->Get_Time() % m_iAttackCount2 == 0)
		{
			m_bCanAttack = true;
		}
	}
}

void COlmec::Attack()
{
	if (m_bCanAttack == true)
	{
		m_eCurState = ATTACK;
		if (m_tFrame.iFrameStart == 3)
		{
			if (m_bFirstAttack == true)
			{
				m_dwTime = GetTickCount();
				m_bFirstAttack = false;
			}
		}
		if (m_dwTime + 1000 < GetTickCount())
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_BOMB, CBombFactory::Create(m_tInfo.fX + TILECX * 1.6f, m_tInfo.fY, false));
			CObjMgr::Get_Instance()->Add_Object(OBJ_BOMB, CBombFactory::Create(m_tInfo.fX - TILECX * 1.6f, m_tInfo.fY, true));
			m_iBombCount++;
			m_dwTime = GetTickCount();
		}
		if (m_iBombCount >= 3)
		{
			m_eCurState = ATTACKEND;
			if (m_tFrame.iFrameStart == 6)
			{
				m_bCanAttack = false;
				m_bCanMove = true;
				m_bFirstAttack = true;
				m_iBombCount = 0;
				m_fPrePlayerX2 = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX;
			}
		}
	}
}

void COlmec::Broken()
{
	m_eCurState = BROKEN;
	if (CLineMgr::Get_Instance()->Collision_Olmec_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
		Break();
	Gravity();
}

void COlmec::Motion_Change() //차후 폭탄 발사 시 울맥 벌어질 때 필요
{
	if (m_ePreState != m_eCurState)
	{
		m_iRepeatCount = 0;
		switch (m_eCurState)
		{
		//case COlmec::CUTSCENE:	Set_Frame(0, 4, 0, false, 150, 0, 4);		break;		//빨리 테스트하기용
		case COlmec::CUTSCENE:	Set_Frame(0, 4, 0, false, 1500, 0, 4);		break;
		case COlmec::IDLE:		Set_Frame(0, 0, 0, true, 120, 0, 7);		break;
		case COlmec::ATTACK:	Set_Frame(0, 3, 0, false, 120, 0, 7);		break;
		case COlmec::ATTACKEND:	Set_Frame(3, 6, 0, false, 120, 0, 7);		break;
		case COlmec::BROKEN:	Set_Frame(7, 7, 0, false, 120, 0, 7);		break;
		}
		m_ePreState = m_eCurState;
	}
}
