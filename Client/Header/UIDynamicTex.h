#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CDynamicRcTex;
class CTexture;
class CTransform;

END

class CUIDynamicTex : public Engine::CGameObject
{
private:
	explicit CUIDynamicTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUIDynamicTex(const CUIDynamicTex& rhs);
	virtual ~CUIDynamicTex();

public:
	virtual HRESULT Ready_GameObject(wstring TextureTag, float fSizeX, float fSizeY, float fX, float fY, float fZ, bool bDirisRight, bool noLines);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)	 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component(wstring TextureTag, bool bDirisRight);

public:
	_float				Get_PosX() { return m_fX; }
	_float				Get_PosY() { return m_fY; }

	void				Set_TexIdx(int index) { m_texIdx = index; }
	int					Get_TexIdx() { return m_texIdx; }

private:
	LPD3DXLINE m_pLine;
	D3DXVECTOR2 vertices[5];
	TCHAR* m_szPrintStr;

	Engine::CDynamicRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

private:
	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	bool				m_noLines = true;
	int					m_texIdx = 0;
public:
	static CUIDynamicTex* Create(LPDIRECT3DDEVICE9	pGraphicDev, wstring TextrueTag, float fSizeX, float fSizeY, float fX, float fY, float fZ, bool bDirisRight, bool noLines = true);

private:
	virtual void Free() override;
};
