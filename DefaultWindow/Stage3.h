#pragma once
#include "Scene.h"

class CStage3 : public CScene
{
public:
	CStage3();
	virtual ~CStage3();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

	void		InsertGimics();
	void		ReleaseGimics();

private:
	void		FadeInOut(HDC hDC);

private:
	void		InsertMonsters();
	void		InsertBmps();
	void		InsertUIs();
	void		InsertObstacles();
	void		InsertBoxs();
	void		InsertChests();
	void		InsertHoldObjs();
	void		InsertLava();
	void		InsertEffects();
	void		InsertItems();

private:
	BLENDFUNCTION	_bf = {};
	float			m_fAlpha = 0.f;
	int				m_fFadeIn = 0;
};
