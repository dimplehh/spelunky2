#include "stdafx.h"
#include "UIMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "Player.h"

CUIMgr::CUIMgr()
{
}


CUIMgr::~CUIMgr()
{
	Release();
}

void CUIMgr::Initialize()
{
	//m_tInfo.fCX = 48.f;
	//m_tInfo.fCY = 48.f;

	//m_pFrameKey = L"UIIcon";

	//m_eRender = RENDER_UI;
}

int CUIMgr::Update()
{

	//__super::Update_Rect();

	return 0;
}

void CUIMgr::Late_Update()
{

}

void CUIMgr::Render(HDC hDC)
{

	TCHAR	szBuff[32] = L"";
	swprintf_s(szBuff, L"플레이어 Hp: %d", dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetHp());
	TextOut(hDC, 10, 20, szBuff, lstrlen(szBuff));


	//HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	//아이콘
	//GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 0 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
	//GdiTransparentBlt(hDC, m_tRect.left + TILECX , m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 1 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
	//GdiTransparentBlt(hDC, m_tRect.left + TILECX * 2, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 2 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
	//GdiTransparentBlt(hDC, WINCX - (int)m_tInfo.fCX * 6 , m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 3 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
	//GdiTransparentBlt(hDC, WINCX - (int)m_tInfo.fCX * 4, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 4 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
	//GdiTransparentBlt(hDC, WINCX - (int)m_tInfo.fCX * 2, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 5 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
}

void CUIMgr::Release()
{
}
