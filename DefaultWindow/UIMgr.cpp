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

void CUIMgr::Release() //Bmp와 달리 객체는 ObjMgr에서 만들어줬기 때문에 따로 delete할 필요 없음
{
	m_mapUI.clear();
}
