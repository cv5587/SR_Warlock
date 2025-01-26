#pragma once
#include "Effect.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
//생성자 바꿔서 텍스처 따로 불러오기
//애니메이션 한번 실행 후 사라지게 하기

class CExplosion : public CEffect
{
private:
	explicit CExplosion(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CExplosion(const CExplosion& rhs);
	virtual ~CExplosion();

public:
	virtual HRESULT Ready_GameObject()                   override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)                override;
	virtual void Render_GameObject()                   override;
	virtual void Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)					override;
	virtual void Action_On_Collision(vector<CLine*> pvecLines)					override;

private:
	HRESULT         Add_Component();

public:
	static CExplosion* Create(LPDIRECT3DDEVICE9   pGraphicDev, _vec3 vPos);

private:
	virtual void Free() override;
	void   TexNamer();
	_bool	m_bDmged;
	_int	m_iFireCount;

};
