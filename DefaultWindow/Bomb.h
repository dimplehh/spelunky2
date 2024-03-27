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
	void		Set_PreY(float _fY) { m_fPreY = _fY; }

public:
	void		Explosion();

private:
	void		SetExplodedTile(int index);
	bool		Gravity();

	float		m_fPower;
	float		m_fTime;

	float		m_fPreY = 0;

	bool		m_bKneelDown = false;

	DWORD		m_dwTime;
	int			m_iBombTime;	//	터지는데 걸리는 시간

	vector<CTile*>* m_pVecTile;
};

