#pragma once
#include "Scene.h"
class CStage :	public CScene
{
public:
	CStage();
	virtual ~CStage();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	void		InsertBmps();
	void		InsertUIs();
	void		InsertBoxs();
	void		InsertItems();

private:
	BLENDFUNCTION _bf = {};
	float			m_fAlpha = 0.f;
};

