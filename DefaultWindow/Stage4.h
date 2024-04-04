#pragma once
#include "Scene.h"

class CStage4 : public CScene
{
public:
	CStage4();
	virtual ~CStage4();

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
	void		InsertBmps();
	void		InsertChests();
	void		InsertHoldObjs();
	void		InsertEffects();

private:
	BLENDFUNCTION	_bf = {};
	float			m_fAlpha = 0.f;
	int				m_fFadeIn = 0;
};
