#pragma once

#include "Base.h"
#include "GameObject.h"

class CMonster;

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CPlayerBullet : public Engine::CGameObject
{
public:
	explicit CPlayerBullet(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed);
	explicit CPlayerBullet(const CPlayerBullet& rhs);
	virtual ~CPlayerBullet();

public:
	virtual HRESULT Ready_GameObject();
	virtual int Update_GameObject(const float& fTimeDelta);
	virtual void LateUpdate_GameObject(const float& fTimeDelta);
	virtual void Set_Pos(_vec3 vPos);
	virtual void Set_Dir(_vec3 Dir);
	PLAYERBULLETTYPE Get_PBType() { return m_ePBType; }
	virtual int Get_Dmg() { return m_iDmg; }

protected:
	virtual HRESULT         Add_Component();

protected:
	D3DXVECTOR3				Calculate_Hit_Point(CGameObject* pTarget);
	D3DXVECTOR3				Calculate_Hit_Point(CLine* pLine);

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	_float m_fSpeed;
	_float m_fFrame;
	_vec3 m_vDir;
	_int m_iDmg;
	_tchar* m_pSound;
	PLAYERBULLETTYPE m_ePBType;
	D3DXVECTOR3		m_vIntersectionPoint;

protected:
	virtual void Free() override;
};