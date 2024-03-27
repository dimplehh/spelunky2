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

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 0 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
	GdiTransparentBlt(hDC, m_tRect.left + TILECX , m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 1 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
	GdiTransparentBlt(hDC, m_tRect.left + TILECX * 2, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 2 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
	GdiTransparentBlt(hDC, WINCX - (int)m_tInfo.fCX * 6 , m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 3 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
	GdiTransparentBlt(hDC, WINCX - (int)m_tInfo.fCX * 4, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 4 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
	GdiTransparentBlt(hDC, WINCX - (int)m_tInfo.fCX * 2, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 5 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
}

void CUIIcon::Release()
{
}
