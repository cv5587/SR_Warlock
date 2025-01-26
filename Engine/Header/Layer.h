#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:
	CComponent*		Get_Component(COMPONENTID eID, const std::wstring pObjTag,wstring pComponentTag);
	CGameObject*	Get_GameObject(const std::wstring pObjTag);

	bool			Is_Monster_Empty() { return m_bPreMonsterEmpty; }
public:
	HRESULT		Add_GameObject(const std::wstring pObjTag, CGameObject* pGameObject);

public:
	HRESULT		Ready_Layer();
	_int		Update_Layer(const _float& fTimeDelta);
	void		LateUpdate_Layer(const float& fTimeDelta);

private:
	multimap<std::wstring, CGameObject*>		m_mapObject;

public:
	static		CLayer*		Create();
	int			m_iMonsterCnt = 0;
	bool		m_bPreMonsterEmpty = false;
private:
	virtual		void		Free();

};

END