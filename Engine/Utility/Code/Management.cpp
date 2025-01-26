#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement(): m_pScene(nullptr)
{
}

CManagement::~CManagement()
{
	Free();
}

CComponent * CManagement::Get_Component(COMPONENTID eID, wstring pLayerTag, wstring pObjTag, wstring pComponentTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
}

CGameObject* CManagement::Get_GameObject(wstring pLayerTag, wstring pObjTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_GameObject(pLayerTag, pObjTag);
}

CLayer* CManagement::Get_Layer(wstring pLayerTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Layer(pLayerTag);
}


HRESULT CManagement::Set_Scene(CScene * pScene)
{
	Safe_Release(m_pScene);

	Engine::Clear_RenderGroup();

	m_pScene = pScene;

	m_iSceneIdx++;

	return S_OK;
}

_int CManagement::Update_Scene(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pScene, -1)



	return m_pScene->Update_Scene(fTimeDelta);
}

void CManagement::LateUpdate_Scene(const float& fTimeDelta)
{
	NULL_CHECK(m_pScene);
	m_pScene->LateUpdate_Scene(fTimeDelta);
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::Render_GameObject(pGraphicDev);

	NULL_CHECK(m_pScene);
	m_pScene->Render_Scene();
}

void CManagement::Free()
{
	Safe_Release(m_pScene);
}
