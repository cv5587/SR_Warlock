#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CBackGround : public Engine::CGameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphicDev, float fSizeX, float fSizeY, float fX, float fY, float fZ);
	explicit CBackGround(const CBackGround& rhs);
	virtual ~CBackGround();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const _float& fTimeDelta)					 override;
	virtual void Render_GameObject()						 override;

public:
	_float				Get_PosX() { return m_fX; }
	_float				Get_PosY() { return m_fY; }
	_float				Get_SizeX() { return m_fSizeX; }
	_float				Get_SizeY() { return m_fSizeY; }

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom[4];

private:
	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float				m_fX, m_fY, m_fZ, m_fSizeX, m_fSizeY;
	TCHAR*				m_szPrintStr[4];
	int					m_PrintTextureIdx;
	int					m_iAlpha;
	float				m_fElipseTime;

public:
	static CBackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev, float fSizeX, float fSizeY, float fX, float fY, float fZ);

private:
	virtual void Free() override;
};

