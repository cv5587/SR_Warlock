#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement :public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	CComponent*			Get_Component(COMPONENTID eID,wstring pLayerTag,wstring pObjTag,wstring pComponentTag);
	CGameObject*		Get_GameObject(wstring pLayerTag, wstring pObjTag);
	CLayer*				Get_Layer(wstring pLayerTag);
public:
	HRESULT			Set_Scene(CScene* pScene);

	_int			Update_Scene(const _float& fTimeDelta);
	void			LateUpdate_Scene(const float& fTimeDelta);
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);


	int				Get_SceneIdx() { return m_iSceneIdx; }

private:
	CScene*		m_pScene;
	int			m_iSceneIdx = 0;

public:
	virtual void Free();
};

END