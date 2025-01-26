#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CProtoMgr : public CBase
{
	DECLARE_SINGLETON(CProtoMgr)

private:
	explicit CProtoMgr();
	virtual ~CProtoMgr();

public:
	HRESULT			Ready_Proto(const wstring& pProtoTag, CComponent* pComponent);
	CComponent*		Clone_Proto(const wstring& pProtoTag);

private:
	CComponent*		Find_Prototype(const wstring& pProtoTag);

private:
	map<const wstring, CComponent*>		m_mapProto;

public:
	virtual void	Free();
};

END