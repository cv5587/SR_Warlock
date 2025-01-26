#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
//생성자 바꿔서 텍스처 따로 불러오기
//애니메이션 한번 실행 후 사라지게 하기

class CEffect : public Engine::CGameObject
{
public:
	explicit CEffect(const CEffect& rhs);

protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev, FXID eID, _vec3 vPos);
	virtual ~CEffect();

public:
	virtual HRESULT Ready_GameObject()                   override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)                override;
	virtual void Render_GameObject()                   override;

private:
	HRESULT         Add_Component();




public:
	static CEffect* Create(LPDIRECT3DDEVICE9   pGraphicDev, FXID eID, _vec3 vPos);

protected:
	virtual void Free() override;
	void   TexNamer();

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
protected:
	FXID m_eFXID;
	wstring m_szTexName;   //텍스처 이름
	_float   m_fFrame = 0.f;
	_int   m_iMaxFrame;
	_float   m_fFrameSpeed;
	_vec3   m_vStart;

};