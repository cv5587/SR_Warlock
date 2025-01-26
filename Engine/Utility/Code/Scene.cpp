#include "..\..\Header\Scene.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

CComponent * CScene::Get_Component(COMPONENTID eID, wstring pLayerTag, wstring pObjTag, wstring pComponentTag)
{
	auto&		iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder2(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;
	
	return iter->second->Get_Component(eID, pObjTag, pComponentTag);
}

CGameObject* CScene::Get_GameObject(wstring pLayerTag, wstring pObjTag)
{
	auto& iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder2(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_GameObject(pObjTag);
}

CLayer* CScene::Get_Layer(wstring pLayerTag)
{
	auto& iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder2(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second;
}

HRESULT CScene::Ready_Scene()
{
	return S_OK;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	for (auto& iter : m_mapLayer)
		iter.second->Update_Layer(fTimeDelta);

	return 0;
}

void CScene::LateUpdate_Scene(const float& fTimeDelta)
{
	for (auto& iter : m_mapLayer)
		iter.second->LateUpdate_Layer(fTimeDelta);
}



void CScene::Free()
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	for (auto iter : m_lLight)
	{
		delete iter;
	}
	m_lLight.clear();

	for (auto iter : m_lMaterial)
	{
		delete iter;
	}	
	m_lMaterial.clear();

	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}
