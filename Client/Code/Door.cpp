#include "stdafx.h"
#include "..\Header\Door.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "GameData.h"
#include "Monster.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev, DOORID eDoorID)
	: CInteractiveProp(pGraphicDev), m_eDoorID(eDoorID), m_bElevate(false), m_bDoorOn(false)
{

}

CDoor::CDoor(const CDoor& rhs)
	: CInteractiveProp(rhs), m_eDoorID(rhs.m_eDoorID), m_bElevate(rhs.m_bElevate), m_bDoorOn(rhs.m_bDoorOn)
{
	Ready_GameObject();
}

CDoor::~CDoor()
{
}

HRESULT CDoor::Ready_GameObject()
{
	TexNamer();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 1.f;
	m_bInit = false;
	m_bIsPlane = true;

	switch (m_eDoorID)
	{
	case DOOR_GREY:
		m_pTransformCom->Set_Pos(2.f, 2.1f, 1.f); //test
		break;
	case DOOR_RED:
		m_pTransformCom->Set_Pos(2.f, 2.1f, 1.f); //test
		break;
	case DOOR_YELLOW:
	case DOOR_BLUE:
	default:
		break;
	}
	m_vPrePos = m_pTransformCom->Get_Pos();
	Push_CollisionMgr(OT_NONPASSPROP, 1.5f);

	return S_OK;
}

HRESULT CDoor::Ready_GameObject(D3DXVECTOR3 vPos, float fYAngle, SpawnGroup eSpawnGroup)
{
	TexNamer();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 1.f;
	m_bInit = false;
	m_bIsPlane = true;
	m_eSpawnGroup = eSpawnGroup;
	m_eObjType = OT_NONPASSPROP;


	m_vPrePos = vPos;
	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Set_Angle({ 0.f, fYAngle, 0.f });
	constexpr float EPSILON = D3DXToRadian(45.f);
	constexpr float PI_HALF = D3DX_PI * 0.5f;

	float fAbsYAngle = abs(fYAngle);

	if (fAbsYAngle >= D3DXToRadian(360.f))
	{
		fAbsYAngle -= D3DXToRadian(360.f);
	}

	if ((fAbsYAngle < EPSILON && 0.f <= fAbsYAngle) ||
		(fAbsYAngle < (D3DX_PI + EPSILON) && (D3DX_PI - EPSILON) < fAbsYAngle) ||
		(fAbsYAngle < 2 * D3DX_PI && (D3DX_PI + PI_HALF + EPSILON) <= fAbsYAngle))
	{
		m_lowerBound = { vPos.x - 1.5f, vPos.y - 1.f, vPos.z - 0.2f };
		m_upperBound = { vPos.x + 1.5f, vPos.y + 1.f, vPos.z + 0.2f };
		m_bIsZDoor = false;
		//Push_CollisionMgr(OT_NONPASSPROP, lowerBound, upperBound);
	}
	else if ((fAbsYAngle < (PI_HALF + EPSILON) && (PI_HALF - EPSILON) < fAbsYAngle) ||
		(fAbsYAngle < (D3DX_PI + PI_HALF + EPSILON) && (D3DX_PI + PI_HALF - EPSILON) < fAbsYAngle))
	{
		m_lowerBound = { vPos.x - 0.2f, vPos.y - 1.f, vPos.z - 1.5f };
		m_upperBound = { vPos.x + 0.2f, vPos.y + 1.f, vPos.z + 1.5f };
		m_bIsZDoor = true;
		//Push_CollisionMgr(OT_NONPASSPROP, lowerBound, upperBound);
	}
	else
	{
		MSG_BOX("Invalid YAngle");
		return E_FAIL;
	}

	m_bColliderIsNone = true;


	return S_OK;
}


Engine::_int CDoor::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bInit)
	{
		m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
		m_bInit = true;
	}

	CGameObject::Update_GameObject(fTimeDelta);


	if (m_bDoorOn)
	{

		Elevate(fTimeDelta);

		if (m_pTransformCom->Get_Pos().y > 10.f)
		{
			return OBJ_DEAD;
		}
	}
	else
	{
		Detect_Player();
	}

	return OBJ_NOEVENT;
}

void CDoor::LateUpdate_GameObject(const float& fTimeDelta)
{

	__super::LateUpdate_GameObject(fTimeDelta);

	_vec3   vPos = m_pTransformCom->Get_Pos();
	__super::Compute_ViewZ(vPos);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

}

void CDoor::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

}

HRESULT CDoor::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_szTexName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ m_szTexName.c_str(), pComponent });

	return S_OK;
}

void CDoor::TexNamer()
{
	switch (m_eDoorID)
	{
	case DOOR_RED:
		m_szTexName = L"Proto_DoorRedTexture";
		break;
	case DOOR_YELLOW:
		m_szTexName = L"Proto_DoorYellowTexture";
		break;
	case DOOR_BLUE:
		m_szTexName = L"Proto_DoorBlueTexture";
		break;
	case DOOR_GREY:
		m_szTexName = L"Proto_DoorGreyTexture";
		break;
	case DOOR_AUTO:
		m_szTexName = L"Proto_DoorNormalTexture";
		break;
	default:
		break;
	}
}

bool CDoor::Elevate(const _float& fTimeDelta)
{
	_vec3 yUp = { 0.f, 1.f, 0.f };
	Move_Pos(yUp, m_fSpeed, fTimeDelta);


	if (2.f > m_pTransformCom->Get_Pos().y)
	{
		return OBJ_NOEVENT;
	}
	else
	{
		return OBJ_DEAD;
	}
}

void CDoor::Detect_Player()
{
	_vec3 vPos, vPlayerPos;
	vPos = m_pTransformCom->Get_Pos();
	vPlayerPos = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos();

	if (DOOR_AUTO != m_eDoorID)
	{
		if (3.f > D3DXVec3Length(&(vPlayerPos - vPos)))
		{
			if (!m_bDoorOn) m_pPlayer->Calculate_Degree(vPos);
			if (m_pPlayer->Get_Key(m_eDoorID) && (Engine::Get_DIKeyState(DIK_SPACE) & 0x80))
			{
				m_bDoorOn = true;
				//여기서 해당 ObjGroup들을 소환 트리거 시키기
				if (m_eSpawnGroup <= 12)
				{
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
				}

				Engine::Get_SoundMgr()->Play_Sound(L"doorUnlock.wav", PROPSOUND, 0.5f);
			}
			else if (!m_pPlayer->Get_Key(m_eDoorID) && (Engine::Get_DIKeyState(DIK_SPACE) & 0x80))
			{
				Engine::Get_SoundMgr()->Play_Sound(L"doorLocked.wav", PROPSOUND, 0.5f);
			}
		}
	}
	else
	{
		if (m_pPlayer->Get_DoorGimmickOn() /*&& Engine::Get_Layer(L"GameLogic")->Is_Monster_Empty()*/) //일단 몬스터 조건 뺌
		{
			m_bDoorOn = true;
		}

		//test
		if (Engine::Get_DIKeyState(DIK_X) & 0x80)
		{
			m_pPlayer->Set_DoorGimmickOn();
		}

	}

}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev, DOORID eDoorID)
{
	CDoor* pInstance = DBG_NEW CDoor(pGraphicDev, eDoorID);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Door Create Failed");
		return nullptr;
	}

	return pInstance;
}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev, DOORID eDoorID, D3DXVECTOR3 vPos, float fYAngle, SpawnGroup eSpawnGroup)
{
	CDoor* pInstance = DBG_NEW CDoor(pGraphicDev, eDoorID);

	if (FAILED(pInstance->Ready_GameObject(vPos, fYAngle, eSpawnGroup)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Door Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CDoor::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{

	if (m_bIsZDoor)
	{
		D3DXVECTOR3 wallNormalDir = { m_upperBound.x - m_lowerBound.x,0.f,0.f };
		D3DXVECTOR3 wallNormalizedDir;
		D3DXVec3Normalize(&wallNormalizedDir, &wallNormalDir);
		D3DXVECTOR3 objDir = pGameObject->Get_TransformCom_Pointer()->Get_Pos() - pGameObject->Get_PrePos();
		float dot = D3DXVec3Dot(&wallNormalizedDir, &objDir);
		pGameObject->Move_Pos(-dot * wallNormalizedDir);
	}
	else
	{
		D3DXVECTOR3 wallNormalDir = { 0.f,0.f,m_upperBound.z - m_lowerBound.z };
		D3DXVECTOR3 wallNormalizedDir;
		D3DXVec3Normalize(&wallNormalizedDir, &wallNormalDir);
		D3DXVECTOR3 objDir = pGameObject->Get_TransformCom_Pointer()->Get_Pos() - pGameObject->Get_PrePos();
		float dot = D3DXVec3Dot(&wallNormalizedDir, &objDir);
		pGameObject->Move_Pos(-dot * wallNormalizedDir);
	}

	pGameObject->LateUpdate_GameObject(0.f);
}

void CDoor::Action_On_Collision(vector<CLine*> pvecLines)
{
}

void CDoor::Free()
{
	__super::Free();
}
