#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CMiniMapUITex : public Engine::CGameObject
{
private:
	explicit CMiniMapUITex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMiniMapUITex(const CMiniMapUITex& rhs);
	virtual ~CMiniMapUITex();

public:
	virtual HRESULT Ready_GameObject(wstring TextureTag, float fSizeX, float fSizeY, float fX, float fY, float fZ, bool noLines);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)	 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component(wstring TextureTag);

public:
	_float				Get_PosX() { return m_fX; }
	_float				Get_PosY() { return m_fY; }

	void				Set_TexIdx(int index) { m_texIdx = index; }
	int					Get_TexIdx() { return m_texIdx; }

private:
	LPD3DXLINE m_pLine;
	D3DXVECTOR2 vertices[5];

	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

private:
	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	bool				m_noLines = true;
	int					m_texIdx = 0;
public:
	static CMiniMapUITex* Create(LPDIRECT3DDEVICE9	pGraphicDev, wstring TextrueTag, float fSizeX, float fSizeY, float fX, float fY,float fZ, bool noLines = true);

private:
	virtual void Free() override;
};

