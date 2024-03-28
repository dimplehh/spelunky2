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

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, m_eUiID * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
	
	TCHAR	szBuff[32] = L"";													//일단 임시로 이걸로 값 출력 (객체 생성시 초기값도 넘겨줘야할것)
	swprintf_s(szBuff, L"%d", m_iNum);
	TextOut(hDC, m_tInfo.fX + 10, m_tInfo.fY + 10, szBuff, lstrlen(szBuff));

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Font");
	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, 16, 16, hMemDC, 0 * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tRect.left + 16 * 1, m_tRect.top, 16, 16, hMemDC, 1 * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tRect.left + 16 * 2, m_tRect.top, 16, 16, hMemDC, 2 * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tRect.left + 16 * 3, m_tRect.top, 16, 16, hMemDC, 3 * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tRect.left + 16 * 4, m_tRect.top, 16, 16, hMemDC, 4 * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tRect.left + 16 * 5, m_tRect.top, 16, 16, hMemDC, 5 * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tRect.left + 16 * 6, m_tRect.top, 16, 16, hMemDC, 6 * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tRect.left + 16 * 7, m_tRect.top, 16, 16, hMemDC, 7 * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tRect.left + 16 * 8, m_tRect.top, 16, 16, hMemDC, 8 * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tRect.left + 16 * 9, m_tRect.top, 16, 16, hMemDC, 9 * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tRect.left + 16 * 10, m_tRect.top, 16, 16, hMemDC, 10 * 16, 0, 16, 16, RGB(63, 63, 63));
	GdiTransparentBlt(hDC, m_tRect.left + 16 * 11, m_tRect.top, 16, 16, hMemDC, 11 * 16, 0, 16, 16, RGB(63, 63, 63));
}

void CUIIcon::Release()
{
}
