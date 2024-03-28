#pragma once

#include "Tile.h"

class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

public:
	void			Picking_Tile(POINT ptMouse, int iDrawID, int iOption, const TCHAR* pframeKey);
	int				Get_Tile_Idx(float _fX, float _fY);
	vector<CTile*>* Get_VecTile() { return &m_vecTile; };
	void			SetDecoLand(HDC hDC, int iScrollX, int iScrollY, int i, int j, int iIndex);

	void		Save_Tile();
	void		Load_Tile();


public:
	static CTileMgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTileMgr;

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
	static CTileMgr*		m_pInstance;
	vector<CTile*>			m_vecTile;
};

