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
}

void CUIIcon::Release()
{
}
