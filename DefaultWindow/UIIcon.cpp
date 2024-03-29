#include "stdafx.h"
#include "UIIcon.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"


CUIIcon::CUIIcon()
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

void CUIIcon::Release()
{
}