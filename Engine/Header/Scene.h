#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene :public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CComponent*			Get_Component(COMPONENTID eID,wstring pLayerTag,wstring pObjTag,wstring pComponentTag);
	CGameObject*		Get_GameObject(wstring pLayerTag, wstring pObjTag);
	CLayer*				Get_Layer(wstring pLayerTag);

public:
	virtual HRESULT		Ready_Scene();
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual void		LateUpdate_Scene(const float& fTimeDelta);
	virtual void		Render_Scene()PURE;


protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	map<wstring, CLayer*>	m_mapLayer;
	list<D3DMATERIAL9*> m_lMaterial;
	list<D3DLIGHT9*> m_lLight;

public:
	virtual void Free();
};

END