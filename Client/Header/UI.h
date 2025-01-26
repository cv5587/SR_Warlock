#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcCol;
class CTexture;
class CTransform;

END

class CUI : public Engine::CGameObject
{
private:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI(const CUI& rhs);
	virtual ~CUI();

public:
	virtual HRESULT Ready_GameObject(float fSizeX, float fSizeY, float fX, float fY, float fZ, bool noLines);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)	 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();

public:
	_float				Get_PosX() { return m_fX; }
	_float				Get_PosY() { return m_fY; }
	_float				Get_SizeX() { return m_fSizeX; }
	_float				Get_SizeY() { return m_fSizeY; }

private:
	LPD3DXLINE m_pLine;
	D3DXVECTOR2 vertices[5];
	TCHAR*		m_szPrintStr;

	Engine::CRcCol* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

private:
	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	bool				m_noLines = true;
public:
	static CUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, float fSizeX, float fSizeY, float fX, float fY,float fZ, bool noLines = true);

private:
	virtual void Free() override;
};

