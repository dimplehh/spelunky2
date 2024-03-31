#include "stdafx.h"
#include "Player.h"
#include "AbstractFactory.h"
#include "Shield.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "Define.h"
#include "Rope.h"
#include "Bomb.h"
#include "HoldObj.h"
#include "UIMgr.h"
#include <iostream>
#include "SoundMgr.h"

float g_fVolume(0.25f);

CPlayer::CPlayer()
	: m_fDistance(0.f), m_bJump(false), m_bLadder(false), m_iJumpCount(0), m_iHp(4), m_fPreY(0.f), m_fCurY(0.f), m_bCanHang(false), m_fDiffY(0.f),
	m_fTime(0.f), m_fPower(0.f), m_ePreState(ST_END), m_eCurState(IDLE), m_bKneelDown(false), m_bAttachedBox(false), m_bIsHold(false), m_bCanHold(false), m_bThrow(false),
	m_dwTime(GetTickCount()), m_fFirstX(TILECX * (21 + m_tInfo.fCX / 2)), m_fFirstY(TILECY * (2 + m_tInfo.fCY / 2))
{
	//ZeroMemory(&m_tPosin, sizeof(POINT));	// 나중에 총구구현에 쓸 수 있어 남겨놓음
	m_eMyObjType = OBJECT_TYPE::PLAYER;
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo		= { m_fFirstX , m_fFirstY, 64.f, 64.f };
	m_fSpeed	= 5.f;
	m_fDistance = 100.f;
	m_fPower = 1.5f;

	m_bCheckFirstInit = true;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Crump_base2.bmp",  L"Player_BASE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Crump_flip2.bmp", L"Player_FLIP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Crump_Attack.bmp", L"Player_ATTACK_BASE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Crump_Attack_Flip.bmp", L"Player_ATTACK_FLIP");
	m_tFrame = { 0, 0, 0, 15, false, 60, GetTickCount(), 0};
	m_pFrameKey = L"Player_BASE";

	m_eRender = RENDER_GAMEOBJECT;

	CSoundMgr::Get_Instance()->PlayBGM(L"Stage1.wav", g_fVolume);
}

int CPlayer::Update()
{
	if (Die())
		return OBJ_NOEVENT;

	Key_Input();
	
	__super::Update_Rect();

	Offset();
	
	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()	//어떤걸 Late_Update, 어떤걸 Update에 넣어야할지 잘 생각해야 할듯
{
	m_bJump = !CLineMgr::Get_Instance()->Collision_Line_Ceiling(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, m_bJump); //천장체킹
	Gravity();
	FallDamage();
	AlmostFell();
	Motion_Change();
	__super::Move_Frame();
	if ((m_tFrame.bRoop == false) && Check_Move_End() == true)
	{
		if (m_bCanHang == true)
			m_eCurState = HANGON;
		else if (m_bIsHold == true)
			m_eCurState = HOLD;
		else
			m_eCurState = IDLE;
	}

#ifdef _DEBUG

	if (m_dwTime + 1000 < GetTickCount())
	{
		cout << m_bCanHold << endl;
		m_dwTime = GetTickCount();
	}
#endif
}

void CPlayer::Release()
{
}

void CPlayer::Render(HDC hDC)
{
	SetRenderImage(hDC);
}

void CPlayer::SetRenderImage(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_eCurState == ATTACK)
	{
		if (m_bFlip == false)	m_pFrameKey = L"Player_ATTACK_BASE";
		else					m_pFrameKey = L"Player_ATTACK_FLIP";

		HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX - 32.f, m_tRect.top + iScrollY - 4, 128, (int)m_tInfo.fCY + 8,
			hMemDC, m_tFrame.iFrameStart * 128, m_tFrame.iMotion * (int)m_tInfo.fCY, 128, (int)m_tInfo.fCY, RGB(62, 62, 62));
	}
	else
	{
		if (m_bFlip == false)	m_pFrameKey = L"Player_BASE";
		else					m_pFrameKey = L"Player_FLIP";
		HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY - 4, (int)m_tInfo.fCX + 8, (int)m_tInfo.fCY + 8,
			hMemDC, m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(62, 62, 62));
	}
}

void CPlayer::Key_Input()
{
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::LEFT) == KEY_STATE::TAP)	{ if (m_bCanHang == true) return;	m_bFlip = true;		m_pFrameKey = L"Player_FLIP";}
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::TAP)	{ if (m_bCanHang == true) return;	m_bFlip = false;	m_pFrameKey = L"Player_BASE";}

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD && CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD) 
	{m_eCurState = IDLE;}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD) { HoldLeft(); }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD) { HoldRight(); }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::LEFT) == KEY_STATE::AWAY) { m_eCurState = IDLE; }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::AWAY) { m_eCurState = IDLE; }

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::Z) == KEY_STATE::TAP) { TapZ(); }
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::C) == KEY_STATE::TAP) { TapC(); }
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::D) == KEY_STATE::TAP) { TapD(); }
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::X) == KEY_STATE::TAP) { TapX(); }
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::A) == KEY_STATE::TAP) { m_eCurState = ENTER; }
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::F) == KEY_STATE::TAP) { m_eCurState = EXIT; }

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::UP) == KEY_STATE::HOLD) { HoldUp(); }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::UP) == KEY_STATE::AWAY) { if (!m_bLadder)		m_eCurState = LOOKFRONT; }

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::DOWN) == KEY_STATE::TAP) { if (!m_bLadder) { m_eCurState = KNEELDOWN;	m_bKneelDown = true; } }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::DOWN) == KEY_STATE::HOLD) { HoldDown(); }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::DOWN) == KEY_STATE::AWAY) { if (!m_bLadder) { m_eCurState = STANDUP;	m_bKneelDown = false; } }
}

void CPlayer::TapX()
{
	if (m_bIsHold == false)
	{
		m_eCurState = ATTACK;
		CSoundMgr::Get_Instance()->PlaySound(L"Attack.wav", SOUND_EFFECT, g_fVolume);
	}
	else if(!(CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::DOWN) == KEY_STATE::HOLD))
	{
		m_eCurState = THROW;
		m_bThrow = true;
	}
}

void CPlayer::TapZ()
{
	if (m_bKneelDown == true)	// 하향점프
	{
		if (CLineMgr::Get_Instance()->Collision_Board_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, m_bJump))
		{
			m_tInfo.fY += 50;
		}
	}
	else
	{
		if (m_iJumpCount < 1) //무한점프 방지
		{
			m_bJump = true;
			m_eCurState = JUMP;
			m_fTime = 0.f;
			m_fPower = 10.f;
			m_bLadder = false;
			m_iJumpCount++;
			if (m_bCanHang == true)
			{
				m_bCanHang = false;
				m_tInfo.fY -= 10;
			}
		}
	}
}

void CPlayer::TapC()
{
	if (SetBombCount(-1) == true)
	{
		m_eCurState = THROW;
		CObjMgr::Get_Instance()->Add_Object(OBJ_BOMB, CAbstractFactory<CBomb>::Create(m_tInfo.fX, m_tInfo.fY));
		CSoundMgr::Get_Instance()->PlaySound(L"Throw2.wav", SOUND_EFFECT, g_fVolume);
	}
	else
		CSoundMgr::Get_Instance()->PlaySound(L"Empty.wav", SOUND_EFFECT, g_fVolume);
}

void CPlayer::TapD()
{
	if (SetRopeCount(-1) == true)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_ROPE, CRopeFactory::Create(m_tInfo.fX, m_tInfo.fY));
		CSoundMgr::Get_Instance()->PlaySound(L"Rope.wav", SOUND_EFFECT, g_fVolume);
	}
	else
		CSoundMgr::Get_Instance()->PlaySound(L"Empty.wav", SOUND_EFFECT, g_fVolume);
}

void CPlayer::HoldLeft()
{
	if (m_bLadder || m_eCurState == ATTACK)
		return;
	if (m_bKneelDown)
	{
		m_eCurState = CRAWL;
		m_tInfo.fX -= m_fSpeed * 0.5f;
	}
	else if (m_bAttachedBox)
	{
		m_eCurState = PUSH;
		m_tInfo.fX -= m_fSpeed * 0.5f;
	}
	else
	{
		if (CLineMgr::Get_Instance()->Collision_Vertical_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))	//벽충돌 판단
		{																											// 벽에 충돌한 상황일 때 매달릴 수 있는지도 판단
			if (m_bCanHang == false && CLineMgr::Get_Instance()->Can_Hang_Line(CLineMgr::Get_Instance()->Get_AttachedLine()->Get_Info().tLPoint.fX,
																				CLineMgr::Get_Instance()->Get_AttachedLine()->Get_Info().tLPoint.fY, 
																				m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, m_bJump))
			{
				m_iJumpCount = 0;
				m_bCanHang = true;
				m_eCurState = HANGON;
			}
		}
		if (!m_bJump && m_bCanHang == false)
		{
			m_eCurState = WALK;
		}
		if(m_bCanHang == false)
			m_tInfo.fX -= m_fSpeed;
	}
}

void CPlayer::HoldRight()
{
	if (m_bLadder || m_eCurState == ATTACK)
		return;
	if (m_bKneelDown)
	{
		m_eCurState = CRAWL;
		m_tInfo.fX += m_fSpeed * 0.5f;
	}
	else if (m_bAttachedBox)
	{
		m_eCurState = PUSH;
		m_tInfo.fX += m_fSpeed * 0.5f;
	}
	else
	{
		if (m_bLadder)
			return;
		if (m_bCanHang == false && CLineMgr::Get_Instance()->Collision_Vertical_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))	//벽충돌 판단
		{
			if (CLineMgr::Get_Instance()->Can_Hang_Line(CLineMgr::Get_Instance()->Get_AttachedLine()->Get_Info().tLPoint.fX, 
														CLineMgr::Get_Instance()->Get_AttachedLine()->Get_Info().tLPoint.fY,
														m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, m_bJump))
			{
				m_iJumpCount = 0;
				m_bCanHang = true;		
				m_eCurState = HANGON;
			}
		}
		if (!m_bJump && m_bCanHang == false)
		{
			m_eCurState = WALK;
		}
		if (m_bCanHang == false)
			m_tInfo.fX += m_fSpeed;
	}
}

void CPlayer::HoldUp()
{
	if (m_bCanHang)
		return;
	InLadder();
	if (m_bLadder)
	{
		m_tInfo.fY -= m_fSpeed;
		m_eCurState = LADDER;
	}
	else
		m_eCurState = LOOKUP;
}

void CPlayer::HoldDown()
{
	if (m_bCanHang)
		return;
	InLadder();
	if (m_bLadder)
	{
		m_tInfo.fY += m_fSpeed;
		m_eCurState = LADDER;
	}
	else
	{
		m_eCurState = KNEELSTAY;
		if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::X) == KEY_STATE::TAP)
		{
			if (m_bCanHold == true)
				m_bIsHold = true;
		}
	}
}

bool CPlayer::Die()
{
	if (m_iHp == 0)
	{
		m_eCurState = DIE;
		if (m_bFirstDieCheck == true)
		{
			m_iDeathTime = CUIMgr::Get_Instance()->Get_Time();
			m_bFirstDieCheck = false;
		}
		else
		{
			Gravity();
			__super::Update_Rect();

			if (m_iDeathTime + 3 <= CUIMgr::Get_Instance()->Get_Time())
			{
				m_tInfo.fX = m_fFirstX;
				m_tInfo.fY = m_fFirstY;

				CScrollMgr::Get_Instance()->Set_ScrollXY(WINCX / 2 - CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX,
					WINCY - -CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY);

				m_eCurState = IDLE;
				SetHp(4);
				ResetNum();
				CUIMgr::Get_Instance()->Reset_Time();
				m_iDeathTime = CUIMgr::Get_Instance()->Get_Time();
				m_bRevival = true;
				m_bFirstDieCheck = true;
				return false;
			}
		}
		return true;
	}
	return false;
}

void CPlayer::FallDamage()
{
	if (m_bJump == false && !CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, m_bJump) && m_eCurState != DIZZY)
	{
		m_eCurState = FALLING;
	}
	else if (m_fDiffY >= TILECY * 6)
	{
		SetHp(-1);
		m_eCurState = DIZZY;
	}
}

void CPlayer::AlmostFell()
{
	if (CLineMgr::Get_Instance()->Check_Almost_Fell(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
	{
		m_bAlmostFell = true;
		if (m_eCurState == IDLE)
			m_eCurState = ALMOSTFELL;
	}
	else
	{
		m_bAlmostFell = false;
	}
}

void CPlayer::InLadder()
{
	if (CLineMgr::Get_Instance()->Ladder_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
	{
		m_iJumpCount = 0;
		m_bLadder = true;
	}
	else
		m_bLadder = false;
}

bool CPlayer::Check_Move_End()
{
	if (m_tFrame.bRoop == false)
	{
		if (m_tFrame.iRepeat == m_iRepeatCount)
			return true;
	}
	return false;
}

void CPlayer::SetHp(int _num)
{
	if (_num == 0)
	{
		m_iHp = 0;
		CUIMgr::Get_Instance()->Set_UINum(CUIIcon::UI_HP, m_iHp);
		m_eCurState = DIZZY;
		return;
	}

	if (m_iHp + _num <= 0)
	{
		m_iHp = 0;
		m_dwTime = GetTickCount();
	}
	else
		m_iHp += _num;

	CUIMgr::Get_Instance()->Set_UINum(CUIIcon::UI_HP, m_iHp);

	if (_num < 0)
		m_eCurState = DIZZY;
}

void CPlayer::Offset()
{
	int		iOffSetminX = WINCX / 2 - TILECX / 2;
	int		iOffSetmaxX = WINCX / 2 + TILECX / 2;

	int		iOffSetminY = WINCY / 2 - TILECY / 2;
	int		iOffSetmaxY = WINCY / 2 + TILECY / 2;

	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_eCurState == LOOKUP || m_eCurState == KNEELSTAY)
	{
		if (Check_Move_End() == true)
		{
			if (m_fMoveOffset < 10.f)
			{
				m_fMoveOffset += 0.3f;
				if (m_eCurState == LOOKUP)
					CScrollMgr::Get_Instance()->Set_ScrollY(m_fMoveOffset);
				else if (m_eCurState == KNEELSTAY)
					CScrollMgr::Get_Instance()->Set_ScrollY(-m_fMoveOffset);
			}
		}
	}
	else
	{
		m_fMoveOffset = 0.f;
		if ((float)iOffSetminX > m_tInfo.fX + fScrollX)
			CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);

		if ((float)iOffSetmaxX < m_tInfo.fX + fScrollX)
			CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed);

		if ((float)iOffSetminY > m_tInfo.fY + fScrollY)
			CScrollMgr::Get_Instance()->Set_ScrollY(m_fSpeed);

		if ((float)iOffSetmaxY < m_tInfo.fY + fScrollY)
			CScrollMgr::Get_Instance()->Set_ScrollY(-m_fSpeed);
	}
}

void CPlayer::Gravity()	//숫자 의미 판단, 더 정리 필요 -> Obj로 나중에 빼야될듯	
{
	for (int i = 0; i < 2; i++)
	{
		if (!m_bLadder && !m_bCanHang && !CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, m_bJump))
		{
			m_fTime += 0.15f;

			float fGravity = m_fPower * m_fTime - ((4.9f * m_fTime * m_fTime) * 0.5f);

			if (fGravity < -5.f)
			{
				fGravity = -5.f;
				m_bJump = false;
			}
			else if (fGravity > 5.f)
				fGravity = 5.f;
			if (fGravity < 0)
				m_bJump = false;

			m_tInfo.fY -= fGravity;
		}
		else
		{
			m_fPower = 0.f;
			m_iJumpCount = 0;
			if (nullptr != CLineMgr::Get_Instance()->Get_AttachedLine())
				m_fCurY = CLineMgr::Get_Instance()->Get_AttachedLine()->Get_Info().tLPoint.fY;
			if (m_fPreY != m_fCurY)
			{
				m_fDiffY = -(m_fPreY - m_fCurY);
				m_fPreY = m_fCurY;
			}
			else
			{
				m_fDiffY = 0;
			}

			return;
		}
	}
}

void CPlayer::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		m_iRepeatCount = 0;
		switch (m_eCurState)
		{
		case CPlayer::IDLE:			Set_Frame(0, 0, 0, false,	60, 0, 15);		break;
		case CPlayer::HOLD:			Set_Frame(11, 11, 2, false, 60, 0, 15);		break;
		case CPlayer::WALK:			Set_Frame(1, 8, 0, true,	30, 0, 15);		break;
		case CPlayer::JUMP:			Set_Frame(0, 11, 9, false,	15, 1, 15);		break;
		case CPlayer::FALLING:		Set_Frame(0, 11, 9, false,	15, 1, 15);		break;
		case CPlayer::DIZZY:		Set_Frame(0, 11, 13, false, 60, 3, 15);		break;
		case CPlayer::DIE:			Set_Frame(9, 9, 0, true,	60, 0, 15);		break;
		case CPlayer::LOOKUP:		Set_Frame(0, 3, 8, false,	60, 0, 15);		break;
		case CPlayer::LOOKFRONT:	Set_Frame(3, 6, 8, false,	60, 1, 15);		break;
		case CPlayer::KNEELDOWN:	Set_Frame(0, 2, 1, false,	60, 1, 15);		break;
		case CPlayer::KNEELSTAY:	Set_Frame(2, 2, 1, false,	60, 1, 15);		break;
		case CPlayer::CRAWL:		Set_Frame(5, 11, 1, true,	60, 0, 15);		break;
		case CPlayer::STANDUP:		Set_Frame(2, 4, 1, false,	60, 1, 15);		break;
		case CPlayer::ATTACKED:		Set_Frame(0, 11, 13, false, 60, 3, 15);		break;
		case CPlayer::ALMOSTFELL:	Set_Frame(0, 7, 3, true,	60, 0, 15);		break;
		case CPlayer::ATTACK:		Set_Frame(0, 7, 0, false,	60, 1, 7);		break;
		case CPlayer::ENTER:		Set_Frame(0, 5, 5, false,	30, 2, 15);		break;
		case CPlayer::EXIT:			Set_Frame(6, 11, 5, false,	30, 2, 15);		break;
		case CPlayer::LADDER:		Set_Frame(0, 5, 6, true,	60, 0, 15);		break;
		case CPlayer::PUSH:			Set_Frame(6, 11, 6, true,	60, 0, 15);		break;	// 상자가 덜덜거리지 않는 구간 - offset에 변화가 없는 구간, 차후 수정 필요
		case CPlayer::HANGON:		Set_Frame(8, 11, 3, false,	100, 1000, 15);	break;
		case CPlayer::THROW:		Set_Frame(6, 10, 4, false,	80, 1, 15);		break;
		}
		m_ePreState = m_eCurState;
	}
}
