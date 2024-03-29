#include "stdafx.h"
#include "BmpMgr.h"
#include "SoundMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "Bomb.h"

extern float g_fVolume;

CBomb::CBomb():m_dwTime(GetTickCount()), m_fTime(0.f)
{
	m_eMyObjType = OBJECT_TYPE::BOMB;
}

CBomb::~CBomb()
{
	Release();
}

void CBomb::Initialize()
{
	m_tInfo = { 0, 0, 48.f, 48.f };
	m_pFrameKey = L"Bomb";
	m_eRender = RENDER_GAMEOBJECT;
	m_fPower = 40.f;
	m_bFlip = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetFlip();
	m_bKneelDown = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetKneelDown();
	m_pVecTile = CTileMgr::Get_Instance()->Get_VecTile();

	m_tFrame = {0, 15, 0, 15, false, 140, GetTickCount(), 0};
}

int CBomb::Update()
{
	if (m_bDead)
		return OBJ_DEAD;


	if(m_dwTime + 3000 < GetTickCount())
	{
		Explosion();
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBomb::Late_Update()
{
	if (!Gravity())
	{
		CLineMgr::Get_Instance()->Collision_Vertical_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY); //이거 작동 안하는거가튼딩;;

		if (m_bKneelDown == false)
		{
			m_fPower = 40.f;
			if (m_bFlip)
				m_tInfo.fX -= 6.f;
			else
				m_tInfo.fX += 6.f;
		}
		else
		{
			m_fPower = 5.f;
			if (m_bFlip)
				m_tInfo.fX -= 7.f;
			else
				m_tInfo.fX += 7.f;
		}

		m_tInfo.fY = m_fPreY - m_fPower * m_fTime + ((9.8f * m_fTime * m_fTime) * 0.5f);
		m_fTime += 0.05f;
	}
	Move_Frame();
}

void CBomb::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(0, 0, 255));
}

void CBomb::Explosion()  // 폭발 범위 인게임과 똑같이 설정
{
	int index = CTileMgr::Get_Instance()->Get_Tile_Idx(m_tInfo.fX, m_tInfo.fY);

	for (int _add = -1; _add <= 1; _add++)
	{
		SetExplodedTile(index + _add - TILEX * 2);	// 맨 윗줄
		SetExplodedTile(index + _add + TILEX);		// 맨 아랫줄
	}
	for (int _add = -2; _add <= 2; _add++)
	{
		SetExplodedTile(index + _add - TILEX);		// 두번째줄
		SetExplodedTile(index + _add);				// 세번째 줄
	}

	CLineMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->SetLine(); //라인 재세팅

	m_bDead = true;

	// 플레이어 체력 깎는 부분. 아직 완벽하진 X

	float _fX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX;
	float _fY = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY;


	if ((*m_pVecTile)[index - 2]->Get_Info().fX - TILECX / 2 <= _fX && _fX <= (*m_pVecTile)[index + 2]->Get_Info().fX + TILECX / 2
		&& (*m_pVecTile)[index - TILEX]->Get_Info().fY - TILECY / 2 <= _fY && _fY <= (*m_pVecTile)[index + TILEX]->Get_Info().fY + TILECY / 2)	//	두번째줄 , 세번째줄에 해당될 때
	{
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetHp(-1);
	}

	if ((*m_pVecTile)[index - 1]->Get_Info().fX - TILECX / 2 <= _fX && _fX <= (*m_pVecTile)[index + 1]->Get_Info().fX + TILECX / 2
		&& (*m_pVecTile)[index - TILEX * 2]->Get_Info().fY - TILECY / 2 <= _fY && _fY <= (*m_pVecTile)[index + TILEX]->Get_Info().fY + TILECY / 2)	// 가운데 세로 3줄
	{
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetHp(-1);
	}

	if ((*m_pVecTile)[index]->Get_Info().fX - TILECX / 2 <= _fX && _fX <= (*m_pVecTile)[index]->Get_Info().fX + TILECX / 2
	&& (*m_pVecTile)[index - TILEX]->Get_Info().fY - TILEY / 2 <= _fY && _fY <= (*m_pVecTile)[index]->Get_Info().fY + TILEY / 2)	//		가장 가운데 2칸
	{
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetHp(-1);													// 여기까지 오면 체력 총 3번 깎임
	}
}

void CBomb::PlayerDamaged()
{

}


void CBomb::SetExplodedTile(int index)
{
	if (0 > index || (size_t)index >= (*m_pVecTile).size())
		return;

	(*m_pVecTile)[index]->Set_FrameKey(L"Tile");
	(*m_pVecTile)[index]->Set_Option(0);
	(*m_pVecTile)[index]->Set_DrawID(0);
}

bool CBomb::Gravity()
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
void CBomb::Release()
{
	CSoundMgr::Get_Instance()->PlaySound(L"Eruption.wav", SOUND_EFFECT, g_fVolume);
}


void CBomb::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		m_tFrame.dwTime = GetTickCount();
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
		{
			if (m_tFrame.bRoop == false)
			{
				if (m_iRepeatCount >= m_tFrame.iRepeat)
				{
					m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
					m_iRepeatCount = m_tFrame.iRepeat;
				}
				else
				{
					m_tFrame.iFrameStart = m_iFirstFrameStart;
					m_iRepeatCount++;
				}
			}
			else if (m_tFrame.bRoop == true)
			{
				m_tFrame.iFrameStart = m_iFirstFrameStart;
			}
		}
	}
}
