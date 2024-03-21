#pragma once
#include "Obj.h"
class CScrewBullet : public CObj
{
public:
	CScrewBullet();
	virtual ~CScrewBullet();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	POINT		m_tCenter;			// 회전 중심점

	float		m_fRotAngle;		// 미사일 회전 각도
	float		m_fRotSpeed;		// 미사일 회전 속도
	float		m_fRotDistance;		// 중점으로부터 떨어진 거리

	bool		m_bStart;
};

