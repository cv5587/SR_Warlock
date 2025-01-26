#pragma once
#include "Effect.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
//������ �ٲ㼭 �ؽ�ó ���� �ҷ�����
//�ִϸ��̼� �ѹ� ���� �� ������� �ϱ�

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
