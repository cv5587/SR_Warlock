#include "stdafx.h"
#include "..\Header\PlayerBullet.h"
#include "Export_Utility.h"
#include "Monster.h"
#include "Player.h"

CPlayerBullet::CPlayerBullet(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed)
	: Engine::CGameObject(pGraphicDev), m_fSpeed(_fSpeed), m_fFrame(0.f), m_iDmg(_iDmg)
	, m_ePBType(PB_END)
{
}

CPlayerBullet::CPlayerBullet(const CPlayerBullet& rhs)
	: Engine::CGameObject(rhs), m_fSpeed(rhs.m_fSpeed), m_fFrame(0.f), m_iDmg(rhs.m_iDmg)
	, m_ePBType(rhs.m_ePBType)
{
	Ready_GameObject();
}

CPlayerBullet::~CPlayerBullet()
{
}

HRESULT CPlayerBullet::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Push_CollisionMgr(OT_PLAYER_BULLET, 0.1f);
	m_fSpeed = 18.f;
	return S_OK;
}

_int CPlayerBullet::Update_GameObject(const float& fTimeDelta)
{
	if (m_bDead)
	{
		switch (m_ePBType)
		{
		case PB_NONE:
			Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"FX_Bullet",
				CEffect::Create(m_pGraphicDev, FX_BULLET, m_vIntersectionPoint));
			break;
		case PB_ROCKET:
			Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"FX_Rocket",
				CEffect::Create(m_pGraphicDev, FX_ROCKET, m_vIntersectionPoint));
			break;
		case PB_STAFF:
			Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"FX_Staff",
				CEffect::Create(m_pGraphicDev, FX_BULLET, m_vIntersectionPoint));
			break;
		case PB_REACTOR:
			Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"FX_Reactor",
				CEffect::Create(m_pGraphicDev, FX_MAGICBLAST, m_vIntersectionPoint));
			break;
		default:
			break;
		}
		return OBJ_DEAD;
	}

	_float fPosY = m_pTransformCom_Clone->Get_Pos().y;
	m_fFrame += 90.f * fTimeDelta;
	if (m_fFrame > 90.f)
	{
		m_vIntersectionPoint = m_pTransformCom->Get_Pos();
		m_bDead = true;
	}
	else if (fPosY < 1.f)
	{
		D3DXVECTOR3 vPos = m_pTransformCom->Get_Pos();
		m_vIntersectionPoint = { vPos.x, 1.1f, vPos.z };
		m_bDead = true;
	}
	else if ((Engine::Get_SceneIdx() - 3) == 0 && fPosY > 3.f)
	{
		D3DXVECTOR3 vPos = m_pTransformCom->Get_Pos();
		m_vIntersectionPoint = { vPos.x, 2.9f, vPos.z };
		m_bDead = true;
	}

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	CGameObject::Update_GameObject(fTimeDelta);
	Move_Pos(m_vDir, m_fSpeed, fTimeDelta);
	m_pTransformCom->Update_Component(fTimeDelta);
	return 0;
}

void CPlayerBullet::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);
}

void CPlayerBullet::Set_Pos(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CPlayerBullet::Set_Dir(_vec3 Dir)
{
	m_vDir = Dir;
}

HRESULT CPlayerBullet::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PBullet"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PBullet", pComponent });

	return S_OK;
}

D3DXVECTOR3 CPlayerBullet::Calculate_Hit_Point(CGameObject* pTarget)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));

	D3DXVECTOR3 targetDir = pTarget->Get_TransformCom_Pointer()->Get_Pos() - pPlayer->Get_TransformCom_Pointer()->Get_Pos();
	D3DXVECTOR3 planeDir;
	D3DXVec3Cross(&planeDir, &_vec3(0.f, 1.f, 0.f), &targetDir);
	D3DXVec3Normalize(&planeDir, &planeDir);

	D3DXVECTOR2 planeDir2D = { planeDir.x, planeDir.z };

	D3DXVECTOR3 p1 = { pTarget->Get_TransformCom_Pointer()->Get_Pos().x,pTarget->Get_TransformCom_Pointer()->Get_Pos().z ,1.f};
	D3DXVECTOR3 p2 = D3DXVECTOR3(planeDir2D.x ,planeDir2D.y,0.f) + p1;

	D3DXVECTOR3 p3 = { pPlayer->Get_TransformCom_Pointer()->Get_Pos().x,pPlayer->Get_TransformCom_Pointer()->Get_Pos().z,1.f };
	D3DXVECTOR3 p4 = { m_pTransformCom->Get_Pos().x,m_pTransformCom->Get_Pos().z,1.f };

	D3DXVECTOR3 cross1, cross2, resCross;
	D3DXVec3Cross(&cross1, &p1, &p2);
	D3DXVec3Cross(&cross2, &p3, &p4);
	D3DXVec3Cross(&resCross, &cross1, &cross2);

	D3DXVECTOR3 intersection;

	intersection.x = resCross.x / resCross.z;
	intersection.y = m_pTransformCom->Get_Pos().y;
	intersection.z = resCross.y / resCross.z;

	D3DXVec3Normalize(&targetDir, &targetDir);

	intersection -= targetDir * 0.1f;

	return intersection;
}

D3DXVECTOR3 CPlayerBullet::Calculate_Hit_Point(CLine* pLine)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));

	D3DXVECTOR3 p1 = { pLine->Get_ArrVtxPos()[0].x,pLine->Get_ArrVtxPos()[0].z,1.f };
	D3DXVECTOR3 p2 = { pLine->Get_ArrVtxPos()[1].x,pLine->Get_ArrVtxPos()[1].z,1.f };

	D3DXVECTOR3 p3 = { pPlayer->Get_TransformCom_Pointer()->Get_Pos().x,pPlayer->Get_TransformCom_Pointer()->Get_Pos().z,1.f };
	D3DXVECTOR3 p4 = { m_pTransformCom->Get_Pos().x,m_pTransformCom->Get_Pos().z,1.f };

	D3DXVECTOR3 cross1, cross2, resCross;
	D3DXVec3Cross(&cross1, &p1, &p2);
	D3DXVec3Cross(&cross2, &p3, &p4);
	D3DXVec3Cross(&resCross, &cross1, &cross2);

	D3DXVECTOR3 intersection;

	D3DXVECTOR3 wallDir = pLine->Get_ArrVtxPos()[1] - pLine->Get_ArrVtxPos()[0];
	D3DXVECTOR3 wallNormalDir;
	D3DXVECTOR3 yDir = { 0.f,1.f,0.f };
	D3DXVec3Cross(&wallNormalDir, &yDir, &wallDir);
	D3DXVec3Normalize(&wallNormalDir, &wallNormalDir);

	intersection.x = resCross.x / resCross.z;
	intersection.y = m_pTransformCom->Get_Pos().y;
	intersection.z = resCross.y / resCross.z;

	intersection += wallNormalDir * 0.1f;

	return intersection;
}

void CPlayerBullet::Free()
{
	__super::Free();
	//Safe_Release(m_pBufferCom);
	//Safe_Release(m_pTransformCom);
	//Safe_Release(m_pTextureCom);
}