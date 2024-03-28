#pragma once

#include "Obj.h"
#include "Define.h"
#include "UIIcon.h"

class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	CObj*		Get_Player() { return m_ObjList[OBJ_PLAYER].front(); }
	CObj*		Get_Bomb() { return m_ObjList[OBJ_BOMB].back(); }
	CObj*		Get_UI() { return m_ObjList[OBJ_UI].back(); }
	CObj*		Get_Target(OBJID eID, CObj* pObj);

public:
	void		Add_Object(OBJID eID, CObj* pObj);
	void		Update();		
	void		Late_Update();
	void		Render(HDC hDC);		
	void		Release();

	void		Delete_ID(OBJID eID);

private:
	list<CObj*>	m_ObjList[OBJ_END];
	list<CObj*>	m_RenderList[RENDER_END];

public:
	static CObjMgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;

		return m_pInstance;
	}
	static void		Destroy()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CObjMgr*		m_pInstance;

};

