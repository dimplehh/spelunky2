#include "stdafx.h"
#include "UIMgr.h"

CUIMgr* CUIMgr::m_pInstance = nullptr;

CUIMgr::CUIMgr():m_dwTime(GetTickCount())
{
}

CUIMgr::~CUIMgr()
{
	Release();
}

void CUIMgr::Late_Update()
{
	if (m_dwTime + 1000 < GetTickCount())
	{
		m_iTime++;
		m_dwTime = GetTickCount();
	}
}

void CUIMgr::Release() //Bmp�� �޸� ��ü�� ObjMgr���� �������� ������ ���� delete�� �ʿ� ����
{
	m_mapUI.clear();
}
