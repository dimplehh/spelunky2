#pragma once

#include "Scene.h"

class CMyEdit :	public CScene
{
public:
	CMyEdit();
	virtual ~CMyEdit();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	void	Key_Input();
	POINT	pt{};
	int		m_iIndex = 1;
	int		m_iOption = 0;
	const TCHAR* m_pFrameKey = L"Tile";
};

