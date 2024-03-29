#pragma once
#include "Obj.h"
class CUIIcon : public CObj
{
public:
	enum UIID {UI_HP, UI_ROPE, UI_BOMB, UI_MONEY, UI_TIME, UI_MAP, UI_END };

public:
	CUIIcon();
	~CUIIcon();

public:
	void	SetUIID(UIID _uiId) { m_eUiID = _uiId; }
	void	Set_Num(int _iNum) { m_iNum = _iNum; }
	void	SetTextXY(float _fX, float _fY) { m_fTextX = _fX; m_fTextY = _fY; }
	void	Set_Size(int _iSize) { m_iSize = _iSize; }

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	void	SetNumberToFont(HDC  hDC);
	void	SetMapFont(HDC hDC);
	void	SetTimeToFont(HDC hDC);

private:
	int		m_iDrawID = 0;
	UIID	m_eUiID = UI_END;
	int		m_iNum = 0;
	int		m_iSize = 0;

	float	m_fTextX = 0;
	float	m_fTextY = 0;

	DWORD	m_dwTime;
};

