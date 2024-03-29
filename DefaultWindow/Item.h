#pragma once
#include "Obj.h"

class CItem : public CObj
{
public:
	enum ITEMID { ITEM_GEM, ITEM_GOLD, ITEM_BOMB, ITEM_ROPE,ITEM_END };

public:
	CItem();
	virtual ~CItem();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	bool		Gravity();

public:
	void	Set_Num(int _num) { m_iNum = _num;}
	int		Get_Num() { return m_iNum; }
	void	Set_ItemID(ITEMID _eId) { m_eItemID = _eId; }
	ITEMID	Get_ItemID() { return m_eItemID; }

private:
	int		m_iNum = 0;

	float				m_fPower;
	float				m_fTime;

	ITEMID				m_eItemID;
};

