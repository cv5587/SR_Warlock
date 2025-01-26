#pragma once

#include "Frame.h"

BEGIN(Engine)

class ENGINE_DLL CFrameMgr : public CBase
{
	DECLARE_SINGLETON(CFrameMgr)

private:
	explicit CFrameMgr();
	virtual ~CFrameMgr();

public:
	_bool			IsPermit_Call(wstring pFrameTag, const _float& fTimeDelta);
	HRESULT			Ready_Frame(wstring pFrameTag, const _float& fCallLimit);
	
private:
	CFrame*			Find_Frame(wstring pFrameTag);

private:
	map<wstring, CFrame*>			m_mapFrame;

private:
	virtual void		Free();
};

END