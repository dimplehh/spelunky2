#pragma once

#include "Obj.h"

class CPlayer :	public CObj
{
public:								//����	//���� ��			// ���帲				//��	//�Ͼ
	enum STATE { IDLE, WALK, JUMP, DIZZY, DIE, LOOKUP, LOOKFRONT, KNEELDOWN, KNEELSTAY, CRAWL, STANDUP, 
					ATTACKED, ALMOSTFELL, ATTACK, ENTER, EXIT, LADDER, PUSH, HANGON, ST_END };
							//������ ��									// �Ŵ޸�
public:
	CPlayer();
	virtual ~CPlayer();
	
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update()override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	void		Key_Input();
	void		Offset();
	void		Motion_Change();

	void		Gravity();
	void		InLadder();

private:

	POINT		m_tPosin;
	float		m_fDistance;

	bool		m_bJump;
	bool		m_bLadder;
	bool		m_bKneelDown; //���帰 ����

	float		m_fPower;
	float		m_fTime;

	STATE		m_ePreState;
	STATE		m_eCurState;
};





