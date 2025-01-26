#include "..\..\Header\GameObject.h"
#include "Export_Utility.h"
#include "Engine_Enum.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_fViewZ(0.f), m_uParticleIdx(99999), m_eObjType(OT_NONE), m_bDead(false), m_fRadius(0.f), m_pTransformCom_Clone(nullptr), m_bIsMovingObj(false), m_bColliderIsNone(false), m_bIsPlane(false),
	m_iLightIndex(-1)
{
	m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev), m_eObjType(rhs.m_eObjType), m_fRadius(rhs.m_fRadius), m_iLightIndex(-1), 
	 m_bIsMovingObj(rhs.m_bIsMovingObj),m_bDead(rhs.m_bDead), m_pTransformCom_Clone(nullptr), m_bColliderIsNone(rhs.m_bColliderIsNone), m_bIsPlane(rhs.m_bIsPlane)
{
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
}



void CGameObject::Push_CollisionMgr(OBJ_TYPE eType, double radius)
{
	if (m_pTransformCom_Clone == nullptr)
	{
		MSG_BOX("pTransformCom_Clone is Not Exist");
		return;
	}

	int particleIdx = Engine::Add_Collider(this,radius);
	D3DXVECTOR3 vPos = m_pTransformCom_Clone->Get_Pos();

	m_lowerBound = vPos - D3DXVECTOR3(radius,radius,radius);
	m_upperBound = vPos + D3DXVECTOR3(radius,radius,radius);

	m_fRadius = radius;
	m_uParticleIdx = particleIdx;
	m_eObjType = eType;
}
//Radius 세팅해주는 부분 없으니 조심
void CGameObject::Push_CollisionMgr(OBJ_TYPE eType, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound)
{
	int particleIdx = Engine::Add_Collider(this,lowerBound,upperBound);
	m_lowerBound = lowerBound;
	m_upperBound = upperBound;

	float maxRadius = 0.f;
	for (int i = 0; i < 3; ++i)
	{
		maxRadius = max(maxRadius, (m_upperBound[i] - m_lowerBound[i]) / 2.f);
	}

	m_fRadius = maxRadius;

	m_uParticleIdx = particleIdx;
	m_eObjType = eType;
}

void CGameObject::Remove_Collider()
{
	if (!m_bColliderIsNone)
	{
		m_bColliderIsNone = true;
		Engine::Remove_Collider(m_uParticleIdx);
	}
}

HRESULT CGameObject::Ready_GameObject()
{
	return S_OK;
}

_int CGameObject::Update_GameObject(const _float & fTimeDelta)
{
	m_vDir = { 0.f,0.f,0.f };
	int iResult = 0;

	if (m_pTransformCom_Clone)
	{
		m_vPrePos = m_pTransformCom_Clone->Get_Pos();
		D3DXVECTOR3 vPos = m_pTransformCom_Clone->Get_Pos();

		D3DXVECTOR3 tmplowerBound = m_lowerBound;
		m_lowerBound = vPos - (m_upperBound - tmplowerBound) / 2.f;
		m_upperBound = vPos + (m_upperBound - tmplowerBound) / 2.f;
	}

	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iResult = iter.second->Update_Component(fTimeDelta);

	return iResult;
}

void CGameObject::LateUpdate_GameObject(const float& fTimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component(fTimeDelta);


	if (m_pTransformCom_Clone)
	{
		D3DXVECTOR3 vPos = m_pTransformCom_Clone->Get_Pos();

		D3DXVECTOR3 tmplowerBound = m_lowerBound;
		m_lowerBound = vPos - (m_upperBound - tmplowerBound) / 2.f;
		m_upperBound = vPos + (m_upperBound - tmplowerBound) / 2.f;
	}


}
void CGameObject::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
	if (m_pTransformCom_Clone == nullptr)
	{
		MSG_BOX("pTransformCom_Clone is Not Exist");
		return;
	}

	if (!m_bIsMovingObj)
		return;

	float otherRadius = pGameObject->Get_Radius();

	D3DXVECTOR3 vPos, vOtherPos;
	m_pTransformCom_Clone->Get_Info(INFO_POS, &vPos);
	pGameObject->Get_TransformCom_Pointer()->Get_Info(INFO_POS, &vOtherPos);
	
	D3DXVECTOR3 normalVec = vPos - vOtherPos;
	float distance = D3DXVec3LengthSq(&normalVec);
	float displacement = m_fRadius + otherRadius - distance;
	D3DXVec3Normalize(&normalVec, &normalVec);

	Move_Pos(normalVec,displacement*20.f,fTimeDelta);

	m_pTransformCom_Clone->Get_Info(INFO_POS, &vPos);
	D3DXVECTOR3 tmplowerBound = m_lowerBound;
	m_lowerBound = vPos - (m_upperBound - tmplowerBound) / 2.f;
	m_upperBound = vPos + (m_upperBound - tmplowerBound) / 2.f;
}

void CGameObject::Action_On_Collision(vector<CLine*> pvecLines)
{
	if (pvecLines.empty())
		return;

	const D3DXVECTOR2 ObjectPos = { m_pTransformCom_Clone->Get_Pos().x, m_pTransformCom_Clone->Get_Pos().z };
	sort(pvecLines.begin(), pvecLines.end(), [&](CLine* line1, CLine* line2)
		{
			float distance1, distance2;
			D3DXVECTOR2 line1_p1 = { line1->Get_ArrVtxPos()[0].x ,line1->Get_ArrVtxPos()[0].z};
			D3DXVECTOR2 line1_p2 = { line1->Get_ArrVtxPos()[1].x ,line1->Get_ArrVtxPos()[1].z};

			distance1 = Engine::distanceToLineSegment(ObjectPos, line1_p1, line1_p2);
			line1->m_fDistance = distance1;

			D3DXVECTOR2 line2_p1 = { line2->Get_ArrVtxPos()[0].x ,line2->Get_ArrVtxPos()[0].z };
			D3DXVECTOR2 line2_p2 = { line2->Get_ArrVtxPos()[1].x ,line2->Get_ArrVtxPos()[1].z };

			distance2 = Engine::distanceToLineSegment(ObjectPos, line2_p1, line2_p2);
			line2->m_fDistance = distance2;

			return distance1 < distance2;
		});

	
	for (vector<CLine*>::iterator it = pvecLines.begin(); it != pvecLines.end(); ++it)
	{

		D3DXVECTOR3 wallDir = (*it)->Get_ArrVtxPos()[1] - (*it)->Get_ArrVtxPos()[0];
		D3DXVECTOR3 wallNormalDir;
		D3DXVECTOR3 yDir = { 0.f,1.f,0.f };
		D3DXVec3Cross(&wallNormalDir, &yDir, &wallDir);
		D3DXVec3Normalize(&wallNormalDir, &wallNormalDir);
		D3DXVECTOR3 objDir = m_pTransformCom_Clone->Get_Pos() - m_vPrePos;
		float dot = D3DXVec3Dot(&wallNormalDir, &objDir);
		if (dot < 0)
		{
				Move_Pos(-dot * wallNormalDir);
		}
	}

	D3DXVECTOR3 vPos = m_pTransformCom_Clone->Get_Pos();
	D3DXVECTOR3 tmplowerBound = m_lowerBound;
	m_lowerBound = vPos - (m_upperBound - tmplowerBound) / 2.f;
	m_upperBound = vPos + (m_upperBound - tmplowerBound) / 2.f;
	LateUpdate_GameObject(0.f);
}

HRESULT CGameObject::Draw_Box_Collider()
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);	
	memcpy(&matWorld.m[INFO_POS][0], &(m_pTransformCom_Clone->Get_Pos()), sizeof(_vec3));
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	if (m_pBoxMesh == NULL)
	{
		float width = m_upperBound.x - m_lowerBound.x;
		float height= m_upperBound.y - m_lowerBound.y;
		float depth = m_upperBound.z - m_lowerBound.z;

		FAILED_CHECK_RETURN(D3DXCreateBox(m_pGraphicDev, width, height, depth, &m_pBoxMesh, NULL), E_FAIL);
	}
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoxMesh->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	return S_OK;
}

void CGameObject::Render_GameObject()
{
}
void CGameObject::Free()
{
	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	if (m_eObjType != OT_NONE && !m_bColliderIsNone)
	{
		Remove_Collider();
	}

	Safe_Release(m_pGraphicDev);
}

CComponent * CGameObject::Find_Component(COMPONENTID eID, wstring pComponentTag)
{
	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder2(pComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;
	
	return iter->second;
}
CComponent * CGameObject::Get_Component(COMPONENTID eID, wstring pComponentTag)
{
	CComponent*		pComponent = Find_Component(eID, pComponentTag);

	NULL_CHECK_RETURN(pComponent, nullptr);
	
	return pComponent;
}

void Engine::CGameObject::Compute_ViewZ(D3DXVECTOR3 pPos)
{
	_matrix		matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_vec3	vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - pPos));
}

void CGameObject::Move_Pos(const D3DXVECTOR3& Disp)
{
	if (m_pTransformCom_Clone == nullptr)
	{
		MSG_BOX("pTransformCom is nullptr");
		return;
	}

	m_bIsMovingObj = true;
	m_vDir += Disp;
	m_pTransformCom_Clone->Move_Pos(Disp);
	m_pTransformCom_Clone->LateUpdate_Component(0.f);

	D3DXVECTOR3 tmpBound = m_lowerBound;
	m_lowerBound = m_pTransformCom_Clone->Get_Pos() - (m_upperBound - tmpBound) * 0.5f;
	m_upperBound = m_pTransformCom_Clone->Get_Pos() + (m_upperBound - tmpBound) * 0.5f;

}

void CGameObject::Move_Pos(const D3DXVECTOR3& Dir, const _float fSpeed, const _float fTimeDelta)
{
	if (m_pTransformCom_Clone == nullptr)
	{
		MSG_BOX("pTransformCom is nullptr");
		return;
	}

	m_bIsMovingObj = true;
	m_vDir += Dir * fSpeed *fTimeDelta;
	m_pTransformCom_Clone->Move_Pos(Dir,fSpeed,fTimeDelta);
	m_pTransformCom_Clone->LateUpdate_Component(fTimeDelta);

	D3DXVECTOR3 tmpBound = m_lowerBound;
	m_lowerBound = m_pTransformCom_Clone->Get_Pos() - (m_upperBound - tmpBound) * 0.5f;
	m_upperBound = m_pTransformCom_Clone->Get_Pos() + (m_upperBound - tmpBound) * 0.5f;
}

