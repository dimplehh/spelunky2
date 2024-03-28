#pragma once

typedef struct UIInfo
{
	float		fX;
	float		fY;

	int			iIndex;

}UIINFO;

class CUIMgr
{
public:
	CUIMgr();
	~CUIMgr();

private:
	int		m_iDrawID = 0;


};

