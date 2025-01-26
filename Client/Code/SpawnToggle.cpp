#include "stdafx.h"
#include "SpawnToggle.h"
#include "Export_Utility.h"
#include "GameData.h"
#include "Monster.h"

CSpawnToggle::CSpawnToggle(const CSpawnToggle& rhs)
	:CGameObject(rhs),m_eSpawnGroup(rhs.m_eSpawnGroup), m_pTransformCom(rhs.m_pTransformCom)
{
}

CSpawnToggle::CSpawnToggle(LPDIRECT3DDEVICE9 pGraphicDev, SpawnGroup eSpawnGroup)
	: CGameObject(pGraphicDev),m_eSpawnGroup(eSpawnGroup), m_pTransformCom(nullptr)
{
}

CSpawnToggle::~CSpawnToggle()
{
}

HRESULT CSpawnToggle::Ready_GameObject(D3DXVECTOR3 vPos, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(vPos);
	m_vPrePos = m_pTransformCom->Get_Pos();
	Push_CollisionMgr(OT_SPAWNTOGGLE, lowerBound, upperBound);

	return S_OK;
}

_int CSpawnToggle::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	int iExit = CGameObject::Update_GameObject(fTimeDelta);
	return iExit;
}

void CSpawnToggle::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);
}

void CSpawnToggle::Render_GameObject()
{
}

void CSpawnToggle::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
	if (pGameObject->Get_ObjType() == OT_PLAYER)
	{
		//Engine::Get_SceneIdx() - 3 == stageScene
		if (CGameData::GetInstance()->IsActiveObjGroup(Engine::Get_SceneIdx() - 3, m_eSpawnGroup) == false)
		{
			const auto& tmpObjGroup = CGameData::GetInstance()->Get_ObjGroup(Engine::Get_SceneIdx() - 3, m_eSpawnGroup);
			CLayer* pLayer = Engine::Get_Layer(L"GameLogic");

			for (auto& PairObj : tmpObjGroup)
			{
				static_cast<CMonster*>(PairObj.second)->Push_CollisionMgr();
				pLayer->Add_GameObject(PairObj.first, PairObj.second);
			}

			CGameData::GetInstance()->Set_ActiveObjGroup(Engine::Get_SceneIdx() - 3, m_eSpawnGroup);
		}
		m_bDead = true;
	}
}

void CSpawnToggle::Action_On_Collision(vector<CLine*> pvecLines)
{
}

HRESULT CSpawnToggle::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CSpawnToggle* CSpawnToggle::Create(LPDIRECT3DDEVICE9 pGraphicDev, SpawnGroup eSpawnGroup, D3DXVECTOR3 vPos, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound)
{
	CSpawnToggle* pInstance = DBG_NEW CSpawnToggle(pGraphicDev, eSpawnGroup);

	if (FAILED(pInstance->Ready_GameObject(vPos, lowerBound, upperBound)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Item Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CSpawnToggle::Free()
{
	__super::Free();
}
