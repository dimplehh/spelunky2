#pragma once
#include "Obj.h"

class CBomb : public CObj
{
public:
	CBomb();
	virtual ~CBomb();

public:
	virtual void Initialize() override;
	virtual int	Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void		Explosion();

private:
	void		SetExplodedTile(int index);
	bool		Gravity();

	float		m_fPower;
	float		m_fTime;

	float		m_fThrowAngle;	//그냥 Obj m_fAngle 써도 될거같긴함

	int			m_iBombTime;	//	터지는데 걸리는 시간

	vector<CTile*>* m_pVecTile;
};

