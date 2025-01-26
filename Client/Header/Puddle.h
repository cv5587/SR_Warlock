#pragma once
#include "Effect.h"
#include <random>
BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
class CPuddle :
    public CEffect
{
private:
	explicit CPuddle(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CPuddle(const CPuddle& rhs);
	virtual ~CPuddle();

public:
	virtual HRESULT Ready_GameObject()                   override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)                override;
	virtual void Render_GameObject()                   override;
	virtual void Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)					override;
	_uint GetRandomInt(_uint lowBound, _uint highBound);

private:
	HRESULT         Add_Component();

public:
	static CPuddle* Create(LPDIRECT3DDEVICE9   pGraphicDev, _vec3 vPos);

private:
	virtual void Free() override;
	void   TexNamer();
	_float m_fAge;
	std::random_device rd;
};

