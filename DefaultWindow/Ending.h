#pragma once
#include "Scene.h"
class CEnding : public CScene
{
public:
	CEnding();
	CEnding(int _money, int _time);
	virtual ~CEnding();

public:
	virtual void Initialize()	override;
	virtual int Update()		override;
	virtual void Late_Update()	override;
	virtual void Render(HDC hDC) override;
	virtual void Release()		 override;

	void		Move_Frame();

	void		SetNumberToFont(int m_iNum, float _fX, float _fY, HDC  hDC);
	void		SetTimeToFont(int m_iTime, float _fX, float _fY, HDC hDC);
	void		SetMapFont(float _fX, float _fY, HDC hDC);

private:
	const TCHAR* m_pFrameKey = L"";
	FRAME		m_tFrame;

	DWORD		m_dwTime;

	int			m_iTime = 0;
	int			m_iMoney = 0;
	int			m_iSize = 16;
};
