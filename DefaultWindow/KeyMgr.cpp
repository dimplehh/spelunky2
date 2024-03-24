#include "stdafx.h"
#include "KeyMgr.h"

int g_arrVK[(UINT)KEY::END] =
{
	VK_LBUTTON,
	VK_RBUTTON,
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	'Q','W','E','R',
	'Y','I','P',
	'A','S','D','F','G',
	'Z','X','C','V','B',

	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_SPACE,
	VK_RETURN,
	VK_ESCAPE,

	VK_F1,
	VK_F2
};



CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}


void CKeyMgr::init()
{
	for (int i = 0; i < (int)KEY::END; ++i)
	{
		m_vecKey.push_back({ KEY_STATE::NONE,false });
	}

}

void CKeyMgr::Update()
{
	for (int i = 0; i < (int)KEY::END; ++i)
	{
		// 지금 키가 눌렸다면
		if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
		{
			// 이전에도 눌렸다
			if (m_vecKey[i].bPrePush)
				m_vecKey[i].eKeyState = KEY_STATE::HOLD;
			else
				m_vecKey[i].eKeyState = KEY_STATE::TAP;

			m_vecKey[i].bPrePush = true;
		}
		else // 지금 안눌렸는데
		{
			// 이전엔 눌렸다
			if (m_vecKey[i].bPrePush)
				m_vecKey[i].eKeyState = KEY_STATE::AWAY;
			else
				m_vecKey[i].eKeyState = KEY_STATE::NONE;

			m_vecKey[i].bPrePush = false;
		}
	}
}
