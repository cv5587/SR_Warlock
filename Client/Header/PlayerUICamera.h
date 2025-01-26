#pragma once

#include "UICamera.h"
#include "Weapon.h"
#include "Engine_Define.h"
#include "Export_Utility.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTransform;

END


enum PLAYERUIDIR
{
	UIDIR_RIGHT,
	UIDIR_LEFT,
	UIDIR_END
};

class CPlayerUICamera : public Engine::CUICamera
{
private:
	explicit CPlayerUICamera(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer);
	explicit CPlayerUICamera(const CPlayerUICamera& rhs);
	virtual ~CPlayerUICamera();

public:
	HRESULT Ready_GameObject(
		const _float& fWidth,
		const _float& fHeight,
		const _float& fPosX,
		const _float& fPosY);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta) override;
	virtual void Render_GameObject();

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	_float				m_fFrame = 0.f;
	CPlayer*			m_pPlayer;

	PLAYERUIDIR			m_eUIDir;
	float				m_fUIAngle;
	D3DXVECTOR3			m_vCentralPos;
	float				m_fUIRadius;
	float				m_fSwingSpeed;

public:
	static CPlayerUICamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _float& fWidth,
		const _float& fHeight,
		const _float& fPosX,
		const _float& fPosY,
		CPlayer*& pPlayer 
	);

private:
	virtual void Free();
};