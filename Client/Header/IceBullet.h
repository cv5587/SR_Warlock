#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

//TODO : 충돌처리 일반 총알과 다르게 PASSPROP으로 ㄱㄱ?
//Flamer, SHambler 사용
class CIceBullet : public Engine::CGameObject
{
protected:
	explicit CIceBullet(LPDIRECT3DDEVICE9 pGraphicDev, _int iTexNum, _vec3 vStart); //TexNum 0 1 2 얼음, 3 화염
	explicit CIceBullet(const CIceBullet& rhs);
	virtual ~CIceBullet();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)	override;
	virtual void Render_GameObject()						 override;
public:

	bool						Get_Dead() { return m_bDead; }
private:
	virtual  HRESULT			Add_Component();

	_float						DistanceToPlayer(CPlayer* pPlayer); // 플레이어 위치 == 카메라 Eye거리 계산

	//밑 두 함수는 업뎃에서 사용
	void						TexNamer();

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

protected:
	CPlayer* m_pPlayer;

	_int				m_iTexNum;
	_vec3				m_vStart;

	wstring m_szTexName;	//텍스처 이름

	_float				m_fFrame = 0.f;
	_uint				m_iMaxFrame = 1;
	_float				m_fFrameSpeed = 0.f;

	_int		m_iDamage;
	_float		m_fSpeed;
	_float		m_fAtkRng;
	_float		m_fAtkTimer;	//데미지 시간 재는 용도 변수

	bool		m_bDead;
	bool		m_bInit;


public:
	static CIceBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iTexNum, _vec3 vStart);

public:
	virtual void Free() override;
};
