#pragma once
#include "Scene.h"
class CLogo : public CScene
{
public:
	CLogo();
	virtual ~CLogo();

public:
	virtual void Initialize()	override;
	virtual int Update()		override;
	virtual void Late_Update()	override;
	virtual void Render(HDC hDC) override;
	virtual void Release()		 override;

	void		Move_Frame();

private:
	const TCHAR* m_pFrameKey = L"";
	FRAME		m_tFrame;
};
