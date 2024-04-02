#pragma once
#include "Obj.h"
#include "Item.h"
#include "HoldObj.h"

class CChest : public CObj
{
public:
	CChest();
	virtual ~CChest();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void	SetChestItemID(CItem::ITEMID _chestItemID) { m_eItemID = _chestItemID; }
	void	SetHoldObjID(CHoldObj::HOLDOBJID _holdObjID) { m_eHoldObjID = _holdObjID; }

private:
	bool		Gravity();

	float				m_fPower;
	float				m_fTime;

private:
	CItem::ITEMID		m_eItemID = CItem::ITEM_END;
	CHoldObj::HOLDOBJID	m_eHoldObjID = CHoldObj::HOLDOBJ_END;
};

