#pragma once
#include "Obj.h"
#include "Item.h"

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

private:
	CItem::ITEMID		m_eItemID = CItem::ITEM_END;
};

