#pragma once

#include "UICamera.h"
#include "Engine_Define.h"
#include "Export_Utility.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CKeyUICamera : public Engine::CUICamera
{
private:
	explicit CKeyUICamera(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer, DOORID eColor);
	explicit CKeyUICamera(const CKeyUICamera& rhs);
	virtual ~CKeyUICamera();

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
	Engine::CTexture* m_pTextureCom;
	_float				m_fFrame = 0.f;
	CPlayer* m_pPlayer;
	DOORID m_eColor;

public:
	static CKeyUICamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _float& fWidth,
		const _float& fHeight,
		const _float& fPosX,
		const _float& fPosY,
		CPlayer*& pPlayer,
		DOORID eColor
	);

private:
	virtual void Free();
};