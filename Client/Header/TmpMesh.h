#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CMesh;

END

class CTmpMesh :
    public CGameObject
{
private:
	explicit CTmpMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTmpMesh(const CTmpMesh& rhs);
	virtual ~CTmpMesh();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();

private:
	Engine::CMesh* m_pMeshCom;

public:
	static CTmpMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

