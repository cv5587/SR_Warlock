#pragma once

#include "Component.h"
#include "Base.h"
#include "DynamicAABBTree.h"

BEGIN(Engine)

class CLine;
class CTransform;


class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject(const float& fTimeDelta);
	virtual void		Render_GameObject();

public:
	_float			Get_ViewZ() { return m_fViewZ; }
	CComponent*		Get_Component(COMPONENTID eID,wstring pComponentTag);
	void			Compute_ViewZ(D3DXVECTOR3 pPos);

public:
	virtual void	Action_On_Collision(CGameObject* pGameObject, float fTimeDelta);
	virtual void	Action_On_Collision(vector<CLine*> pvecLines);
	OBJ_TYPE		Get_ObjType() { return m_eObjType; }

	HRESULT			Draw_Box_Collider();
	ID3DXMesh*		m_pBoxMesh = NULL;

public:

	std::pair<D3DXVECTOR3, D3DXVECTOR3>	Get_Bounds() { return std::pair<D3DXVECTOR3, D3DXVECTOR3>(m_lowerBound, m_upperBound); }
	void Set_Bound(const D3DXVECTOR3& lowerBound, const D3DXVECTOR3& upperBound)
	{
		m_lowerBound = lowerBound;
		m_upperBound = upperBound;

		float maxRadius = 0.f;
		for (int i = 0; i < 3; ++i)
		{
			maxRadius = max(maxRadius, (m_upperBound[i] - m_lowerBound[i]) / 2.f);
		}

		m_fRadius = maxRadius;
	}

public:

	_float			Get_Radius() { return m_fRadius; }
	void			Set_Radius(_float fRadius) { m_fRadius = fRadius; }

	void			Set_ParticleIdx(unsigned int uParticleIdx) { m_uParticleIdx = uParticleIdx; }
	unsigned int	Get_ParticleIdx() { return m_uParticleIdx; }


	//정육면체의 모서리의 길이의 절반을 Collision Detect 범위로 잡겠다!
	void			Push_CollisionMgr(OBJ_TYPE eType, double radius = 1.f);
	//직육면체의 가장 위치가 낮은것이 lowerBound, 가장 위치가 높은 것이 upperBound!
	//Radius 세팅해주는 부분 없으니까 조심
	void			Push_CollisionMgr(OBJ_TYPE eType, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound);

	void			Push_CollisionMgr()
	{
		m_bColliderIsNone = false;
		CGameObject::Push_CollisionMgr(m_eObjType, m_lowerBound, m_upperBound);
	}


	void			Remove_Collider();

public:
	void					Move_Pos(const D3DXVECTOR3& Disp);
	void					Move_Pos(const D3DXVECTOR3& Dir, const _float fSpeed, const _float fTimeDelta);
	
	void					Set_Dir(const D3DXVECTOR3& vDir) { m_vDir = vDir; }
	const D3DXVECTOR3&		Get_Dir() { return m_vDir; }

	const D3DXVECTOR3&		Get_PrePos() { return m_vPrePos; }
	void					Set_PrePos(const D3DXVECTOR3& vPrePos) { m_vPrePos = vPrePos; }

	void					Set_Dead() { m_bDead = true; }

	void					Set_TransformCom_Pointer(CTransform* pTransformCom) { m_pTransformCom_Clone = pTransformCom; }
	CTransform*				Get_TransformCom_Pointer() { return m_pTransformCom_Clone; }

	void					Set_IsColliderNone() { m_bColliderIsNone = true; }


	bool					Is_Plane() { return m_bIsPlane; }
protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<wstring, CComponent*>			m_mapComponent[ID_END];


	D3DXVECTOR3							m_vPrePos;
	_float								m_fViewZ;
	OBJ_TYPE							m_eObjType;
	_float								m_fRadius;

	_int								m_iLightIndex;
	D3DXVECTOR3							m_vDir;

	D3DXVECTOR3							m_lowerBound;
	D3DXVECTOR3							m_upperBound;
	_bool								m_bDead;

	CTransform*							m_pTransformCom_Clone;

	bool								m_bIsMovingObj;
	bool								m_bIsPlane;
	bool								m_bColliderIsNone;

private:
	unsigned int						m_uParticleIdx;


	

private:
	CComponent*		Find_Component(COMPONENTID eID,wstring pComponentTag);

public:
	virtual void		Free();
};

END