#pragma once
#include "Base.h"
#include "Timer.h"

BEGIN(Engine)

class ENGINE_DLL CTimerMgr : public CBase
{

	DECLARE_SINGLETON(CTimerMgr)
private:
	explicit CTimerMgr();
	virtual ~CTimerMgr();

public:
	_float		Get_TimeDelta(wstring pTimerTag);
	void		Update_TimeDelta(wstring pTimerTag);

public:
	HRESULT		Ready_Timer(wstring pTimerTag);

private:
	CTimer*		Find_Timer(wstring pTimerTag);

private:
	map<wstring, CTimer*>		m_mapTimers;

private:
	virtual void		Free(void);
};

END