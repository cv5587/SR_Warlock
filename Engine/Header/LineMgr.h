#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CLine;

class ENGINE_DLL CLineMgr : public CBase
{
private:
	explicit CLineMgr();
	virtual ~CLineMgr();

public:
	void		Push_Line_Collision();
	HRESULT		Remove_All_Line_Component();
private:
	// ¸Ê ¿©·¯°³ ´ëºñ. line number°¡ ÇÑ ¸ÊÀÇ ¿Ü°û¼± °³¼ö, linegroupcount°¡ ¸ÊÀÇ °³¼ö 
	HRESULT		Ready_LineMgr(std::vector<unsigned int> vlineGroup);
	HRESULT		Add_Line_Component(std::vector<unsigned int> vlineGroup);

	


public:
	static CLineMgr* Create(LPDIRECT3DDEVICE9 pGraphicDev, std::vector<unsigned int> vlineGroup);
	
private:
	virtual void	Free();
	static int		totalLineidx;
private:
	vector<CLine*>		m_LineList;


};

END