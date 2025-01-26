#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CMyFont :	public CBase
{
private:
	explicit CMyFont(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMyFont();

public:
	HRESULT		Ready_Font(wstring pFontType, 
							const _uint& iWidth, 
							const _uint& iHeight, 
							const _uint& iWeight);

	void		Render_Font(wstring pString,
							const _vec2* pPos,
							D3DXCOLOR Color);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPD3DXFONT					m_pFont;
	LPD3DXSPRITE				m_pSprite;

public:
	static CMyFont*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
							wstring pFontType, 
							const _uint& iWidth, 
							const _uint& iHeight,
							const _uint& iWeight);
private:
	virtual void	Free();
};

END