#include "stdafx.h"
#include "UIIcon.h"
#include "UIMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"


CUIIcon::CUIIcon() : m_dwTime(GetTickCount())
{
}


CUIIcon::~CUIIcon()
{
	Release();
}

void CUIIcon::Initialize()
{
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;
	
	m_iSize = 16.f;

	m_pFrameKey = L"UIIcon";

	m_eRender = RENDER_UI;
}

int CUIIcon::Update()
{

	__super::Update_Rect();

	return 0;
}

void CUIIcon::Late_Update()
{
}

void CUIIcon::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, m_eUiID * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(38, 38, 38));

	if (m_eUiID == UI_TIME)
		SetTimeToFont(hDC);
	else if (m_eUiID == UI_MAP)
		SetMapFont(hDC);
	else
		SetNumberToFont(hDC);
}

void CUIIcon::SetNumberToFont(HDC  hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Font");

	if (m_iNum < 10)
		GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, (m_iNum % 10) * 16, 0, 16, 16, RGB(63, 63, 63));
	else
	{
		int iTemp = m_iNum;
		int addX = 0;
		while (1)
		{
			GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX - 8.f * addX, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, (iTemp % 10) * 16, 0, 16, 16, RGB(63, 63, 63));
			iTemp = iTemp / 10;
			addX++;
			if (iTemp == 0)
				break;
		}
	}
}

void CUIIcon::SetMapFont(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Font");

	int alpha = 0;
	if (m_iNum == 4) alpha = 1;

	GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX + 10.f * 0, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, ((m_iNum - alpha) % 10) * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX + 10.f * 1, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, 11 * 16, 0, 16, 16, RGB(63, 63, 63));

	GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX + 10.f * 2, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, (1 + alpha) * 16, 0, 16, 16, RGB(63, 63, 63));
}

void CUIIcon::SetTimeToFont(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Font");
	
	int _min = CUIMgr::Get_Instance()->Get_Time() / 60;
	int _sec = CUIMgr::Get_Instance()->Get_Time() % 60;

	GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX + 10.f * 0, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, (_min / 10) * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX + 10.f * 1, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, (_min % 10) * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX + 10.f * 2, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC,			10 * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX + 10.f * 3, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, (_sec / 10) * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX + 10.f * 4, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, (_sec % 10) * 16, 0, 16, 16, RGB(63, 63, 63));
}

void CUIIcon::Release()
{
}