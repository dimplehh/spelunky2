#include "stdafx.h"
#include "UIMgr.h"

CUIMgr* CUIMgr::m_pInstance = nullptr;

CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{
	Release();
}

void CUIMgr::Release() //Bmp�� �޸� ��ü�� ObjMgr���� �������� ������ ���� delete�� �ʿ� ����
{
	m_mapUI.clear();
}
