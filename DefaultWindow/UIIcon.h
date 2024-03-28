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

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	int		m_iDrawID = 0;
	UIID	m_eUiID = UI_END;
	int		m_iNum = 0;
};

