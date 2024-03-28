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

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	int		m_iDrawID = 0;
	UIID	m_eUiID = UI_END;
};

