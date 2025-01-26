#include "..\..\Header\Layer.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

CComponent * CLayer::Get_Component(COMPONENTID eID, const std::wstring pObjTag, wstring pComponentTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder2(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;
	
	return iter->second->Get_Component(eID, pComponentTag);
}

CGameObject* CLayer::Get_GameObject(const std::wstring pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder2(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second;
}

HRESULT CLayer::Add_GameObject(const std::wstring pObjTag, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_mapObject.insert({ pObjTag, pGameObject });

	return S_OK;
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float& fTimeDelta)
{
	int		iResult = 0;
	m_iMonsterCnt = 0;
	for (auto it = m_mapObject.begin(); it != m_mapObject.end();)
	{
		auto range = m_mapObject.equal_range(it->first);
		for (auto it2 = range.first; it2 != range.second;)
		{
			iResult = it2->second->Update_GameObject(fTimeDelta);

			if (it2->second->Get_ObjType() == OT_MONSTER)
				m_iMonsterCnt++;

			if (iResult & 0x80000000) //이거 뭐임??
				return iResult;

			if (OBJ_DEAD == iResult) //OBJ_DEAD return 시 레이어에서 삭제
			{
				Safe_Release(it2->second);
				it2 = m_mapObject.erase(it2);
			}
			else
			{
				++it2;
			}
		}
		it = range.second;

	}
	return iResult;
}

void CLayer::LateUpdate_Layer(const float& fTimeDelta)
{
	if (m_iMonsterCnt == 0)
		m_bPreMonsterEmpty = true;
	else
		m_bPreMonsterEmpty = false;

	for (auto& iter : m_mapObject)
		iter.second->LateUpdate_GameObject(fTimeDelta);
}

CLayer * CLayer::Create()
{
	CLayer *		pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
	{
		Safe_Release(pLayer);

		MSG_BOX("Layer Create Failed");
		return nullptr;
	}
	return pLayer;
}

void CLayer::Free()
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}